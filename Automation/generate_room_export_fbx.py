import argparse
import os
import unreal
import time

def printer(output):
    # print(output)
    unreal.log_warning(output)

def load_level(level_name):
    """Load a level by its name."""
    level_path = '/Game/Levels/' + level_name
    if unreal.EditorAssetLibrary.does_asset_exist(level_path):
        loaded = unreal.EditorLoadingAndSavingUtils.load_map(level_path)
        if loaded:
            printer("Level loaded successfully: {}".format(level_name))
        else:
            printer("Failed to load level: {}".format(level_name))
    else:
        printer("Level does not exist: {}".format(level_path))

def export_selected_fbx(file_path):
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    task = unreal.AssetExportTask()
    task.set_editor_property('selected', True)
    task.set_editor_property('filename', file_path)
    task.set_editor_property('automated', True)
    task.set_editor_property('object', world)

    fbx_options = unreal.FbxExportOption()
    #TODO: do we have a requirement for 'fbx_export_compatibility'?
    #TODO: do we need 'vertex_color'?
    #TODO: other things? Refer to:
    #https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/FbxExportOption?application_version=5.5
    fbx_options.set_editor_property('collision', False)
    fbx_options.set_editor_property('force_front_x_axis', False)
    fbx_options.set_editor_property('level_of_detail', False)
    task.set_editor_property('options', fbx_options)

    result = unreal.Exporter.run_asset_export_task(task)
    print('Export Result: {}'.format(result))

def main():
    parser = argparse.ArgumentParser(description='Process some variables and paths.')
    parser.add_argument('--level_name', type=str, help='Name of the level')
    parser.add_argument('--export_filepath', type=str, help='Path where the export should be saved')

    args = parser.parse_args()

    level_name = args.level_name
    if not level_name:
        printer("Level Name is not provided. Quitting.")
        quit()

    export_filepath = args.export_filepath
    if not export_filepath:
        printer("Export File Path is not provided. Quitting.")
        quit()

    print("Level Name:", level_name)
    print("Export File Path:", export_filepath)

    #
    load_level(level_name)

    #TODO: find a class and call a function from that class before selecting and exporting

    editor_actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    editor_actor_subsystem.select_nothing()
    selected_actors = editor_actor_subsystem.get_all_level_actors()
    editor_actor_subsystem.set_selected_level_actors(selected_actors)

    if len(selected_actors) == 0:
        printer("No actor selected, nothing to export")
        quit()

    # Optional:
    # for actor in selected_actors:
    #     print(actor.get_name())

    if not export_filepath.endswith('/'):
            export_filepath += '/'

    export_filepath += level_name + '.fbx'

    export_selected_fbx(export_filepath)

if __name__ == '__main__':
    main()
