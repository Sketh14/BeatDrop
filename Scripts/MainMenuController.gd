extends Node2D

var startDropPos
var endDropPos
var startBucketPos
var endBucketPos
var time_taken = 0.0
@export var drop : Control
@export var bucket : Control
@export var speedMult : float = 1.0

@export var creditsBt : Button
@export var creditsPanel : Panel

#Start Screen Panel
@export var startScreenBtControl : Control

#Scene Selection
@export var scenePanel : Panel
@export var nextSceneBt : Button
@export var prevSceneBt : Button
@export var currentSceneImg : TextureRect
@export var scenesAvailableImg : Array[CompressedTexture2D]

func _on_playLevel_bt_pressed():
	get_tree().change_scene_to_file("res://Scenes/MainGame.tscn")

func _on_exit_bt_pressed():
	get_tree().quit()
	
func _process(delta):
	time_taken += delta * speedMult

	if (time_taken > 1):
		time_taken = 0
		var tempPos = startBucketPos
		startBucketPos = endBucketPos
		endBucketPos = tempPos

	#drop.set_global_position(lerp(start_pos, end_pos, time_taken))
	#print($"../Control/BG_Drop".position)
	drop.position = Vector2(lerp(startDropPos, endDropPos, time_taken))
	bucket.position = Vector2(lerp(startBucketPos, endBucketPos, time_taken))

func _ready():
	startDropPos = Vector2(205,262)
	endDropPos = Vector2(238,703)
	startBucketPos = Vector2(66,715)
	endBucketPos = Vector2(120,684)
	#drop = $"../Control/BG_Drop" as Control
	#print($"../Control/BG_Drop".position)

func _on_credits_bt_pressed():
	creditsBt.hide()
	creditsPanel.show()

func _on_credtis_close_button_pressed():
	creditsBt.show()
	creditsPanel.hide()

func _on_nextScene_button_pressed():
	nextSceneBt.hide()
	prevSceneBt.show()
	GloballyAccessible.scene2Selected = true
	currentSceneImg.texture = scenesAvailableImg[1]

func _on_previousScene_button_pressed():
	nextSceneBt.show()
	prevSceneBt.hide()
	GloballyAccessible.scene2Selected = false
	currentSceneImg.texture = scenesAvailableImg[0]

func _on_start_bt_pressed():
	scenePanel.show()
	startScreenBtControl.hide()

func _on_exit_scene_selection_bt_pressed():
	scenePanel.hide()
	startScreenBtControl.show()
