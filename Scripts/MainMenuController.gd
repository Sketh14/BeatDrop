extends CanvasLayer


func _on_start_bt_pressed():
	get_tree().change_scene_to_file("res://Scenes/MainGame.tscn")

func _on_exit_bt_pressed():
	get_tree().quit()
