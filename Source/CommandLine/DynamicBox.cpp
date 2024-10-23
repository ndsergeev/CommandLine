#include "DynamicBox.h"

#include "DynamicMeshToMeshDescription.h"
#include "StaticMeshAttributes.h"
#include "UObject/SavePackage.h"

using namespace UE::Geometry;

bool ADynamicBox::SaveStaticMeshAsset(UStaticMesh* NewStaticMesh, const FString& AssetPath, const FString& AssetName)
{
#ifdef UE_EDITOR
	if (NewStaticMesh)
	{
		// const FString FullPackageName = FPaths::Combine(AssetPath, AssetName);
		const FString FullPackageName = FPaths::Combine(TEXT("/Game/GeneratedMeshes"), AssetName);

		UPackage* Package = CreatePackage(*FullPackageName);
		if (!Package)
		{
			return false;
		}

		NewStaticMesh->SetFlags(RF_Public | RF_Standalone);

		NewStaticMesh->Rename(*AssetName, Package, REN_DontCreateRedirectors | REN_NonTransactional);

		FString PackageFileName = FPackageName::LongPackageNameToFilename(
			FullPackageName, FPackageName::GetAssetPackageExtension());
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;
		UPackage::SavePackage(Package, NewStaticMesh, *PackageFileName, SaveArgs);
		return true;
	}
#endif
	return false;
}

ADynamicBox::ADynamicBox()
{
	PrimaryActorTick.bCanEverTick = false;

	DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("Dynamic Mesh Component"));
	DynamicMeshComponent->SetCastShadow(false);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetCastShadow(false);

	GenerateCube(DynamicMeshComponent);
}

void ADynamicBox::BeginPlay()
{
	Super::BeginPlay();

	if (DynamicMeshComponent == nullptr)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		                                 FString::Printf(TEXT("DynamicMeshComponent is null")));
#endif

		return;
	}

	GenerateCube(DynamicMeshComponent);
	
	TargetMesh = DynamicMeshComponent->GetMesh();
	if (TargetMesh == nullptr)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("TargetMesh is null")));
#endif
		return;
	}

	UStaticMesh* StaticMesh = ConvertDynamicToStaticMesh(*TargetMesh, this, FName("GeneratedStaticMesh"));
	if (StaticMesh != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		if (StaticMesh != nullptr)
		{
			SaveStaticMeshAsset(StaticMesh, "", "Shape_Creation_Test_StaticMesh");
		}
	}
}

void ADynamicBox::GenerateCube(UDynamicMeshComponent* MeshComponent)
{
	FDynamicMesh3* DynamicMesh3 = new FDynamicMesh3(true, true, true, false);

	if (!bUseRandomSize)
	{
		constexpr double Size = 100;
		// Bottom vertices
		DynamicMesh3->AppendVertex(FVector3d(0, 0, 0));
		DynamicMesh3->AppendVertex(FVector3d(Size, 0, 0));
		DynamicMesh3->AppendVertex(FVector3d(Size, Size, 0));
		DynamicMesh3->AppendVertex(FVector3d(0, Size, 0));

		// Top vertices
		DynamicMesh3->AppendVertex(FVector3d(0, 0, Size));
		DynamicMesh3->AppendVertex(FVector3d(Size, 0, Size));
		DynamicMesh3->AppendVertex(FVector3d(Size, Size, Size));
		DynamicMesh3->AppendVertex(FVector3d(0, Size, Size));
	}
	else
	{
		// Randomize dimensions
		double SizeX1 = FMath::RandRange(minRandomRange, maxRandomRange);
		double SizeX2 = FMath::RandRange(minRandomRange, maxRandomRange);
		double SizeY1 = FMath::RandRange(minRandomRange, maxRandomRange);
		double SizeY2 = FMath::RandRange(minRandomRange, maxRandomRange);
		double SizeZ = FMath::RandRange(minRandomRange, maxRandomRange);

		// Bottom vertices
		DynamicMesh3->AppendVertex(FVector3d(0, 0, 0));  // Vertex 0
		DynamicMesh3->AppendVertex(FVector3d(SizeX1, 0, 0));  // Vertex 1
		DynamicMesh3->AppendVertex(FVector3d(SizeX2, SizeY1, 0));  // Vertex 2
		DynamicMesh3->AppendVertex(FVector3d(0, SizeY2, 0));  // Vertex 3

		// Top vertices
		DynamicMesh3->AppendVertex(FVector3d(0, 0, SizeZ));  // Vertex 4
		DynamicMesh3->AppendVertex(FVector3d(SizeX1, 0, SizeZ));  // Vertex 5
		DynamicMesh3->AppendVertex(FVector3d(SizeX2, SizeY1, SizeZ));  // Vertex 6
		DynamicMesh3->AppendVertex(FVector3d(0, SizeY2, SizeZ));  // Vertex 7
	}

	TArray Indices = {
		UE::Geometry::FIndex3i(0, 1, 2), UE::Geometry::FIndex3i(0, 2, 3), // Bottom face
		UE::Geometry::FIndex3i(4, 6, 5), UE::Geometry::FIndex3i(4, 7, 6), // Top face
		UE::Geometry::FIndex3i(0, 4, 1), UE::Geometry::FIndex3i(1, 4, 5), // Front face
		UE::Geometry::FIndex3i(1, 5, 6), UE::Geometry::FIndex3i(1, 6, 2), // Right face
		UE::Geometry::FIndex3i(2, 6, 7), UE::Geometry::FIndex3i(2, 7, 3), // Back face
		UE::Geometry::FIndex3i(3, 7, 4), UE::Geometry::FIndex3i(3, 4, 0), // Left face
	};

	for (UE::Geometry::FIndex3i& Tri : Indices)
	{
		DynamicMesh3->AppendTriangle(Tri);
	}

	MeshComponent->SetMesh(MoveTemp(*DynamicMesh3));
}

UStaticMesh* ADynamicBox::ConvertDynamicToStaticMesh(const FDynamicMesh3& DynamicMesh, UObject* InOuter,
                                                     FName InName)
{
	FMeshDescription MeshDescription;
	FStaticMeshAttributes StaticMeshAttributes(MeshDescription);
	StaticMeshAttributes.Register();

	FDynamicMeshToMeshDescription Converter;
	Converter.Convert(&DynamicMesh, MeshDescription);
	TArray<const FMeshDescription*> MeshDescriptionPtrs;
	MeshDescriptionPtrs.Emplace(&MeshDescription);

	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(InOuter, InName, RF_Public | RF_Standalone);
	StaticMesh->BuildFromMeshDescriptions(MeshDescriptionPtrs);

	return StaticMesh;
}
