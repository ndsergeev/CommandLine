#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "GameFramework/Actor.h"
#include "Components/DynamicMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "DynamicBox.generated.h"

using namespace UE::Geometry;

UCLASS()
class COMMANDLINE_API ADynamicBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicBox();

protected:
	UStaticMesh* ConvertDynamicToStaticMesh(const FDynamicMesh3& DynamicMesh, UObject* InOuter, FName InName);

	UFUNCTION(BlueprintCallable, Category = "Mesh Conversion")
	bool SaveStaticMeshAsset(UStaticMesh* NewStaticMesh, const FString& AssetPath, const FString& AssetName);

	UFUNCTION(BlueprintCallable, Category = "Mesh Conversion")
	void GenerateCube(UDynamicMeshComponent* MeshComponent);

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* TargetStaticMesh;

	FDynamicMesh3* TargetMesh;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UDynamicMeshComponent* DynamicMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	bool bUseRandomSize = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	float minRandomRange = 50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	float maxRandomRange = 100;
};
