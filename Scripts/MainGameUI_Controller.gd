extends CanvasLayer

var pauseStatus
@export var localGameManager : Node
@export var localSoundManager : Node
@export var mainGAmeUI : CanvasLayer
#@export var soundManager : Node
@export var pauseBt : Button
@export var pausePanel : Panel
@export var gameOverPanel : Panel
@export var totalScore : Label

# Called when the node enters the scene tree for the first time.
func _ready():
	pauseStatus = false
	#soundManager.process_mode = Node.PROCESS_MODE_ALWAYS

func _on_pause_button_pressed():
	pauseStatus  = !pauseStatus
	get_tree().paused = pauseStatus
	self.process_mode = Node.PROCESS_MODE_WHEN_PAUSED if pauseStatus else Node.PROCESS_MODE_ALWAYS
	if (pauseStatus):
		pauseBt.hide()
		pausePanel.show()
	else:
		pauseBt.show()
		pausePanel.hide()
#	print("pauseStatus : ", pauseStatus)

func _on_main_menu_button_pressed():
	var sfxPlayer = localSoundManager.get_child(1) as AudioStreamPlayer
	sfxPlayer.stop()
	get_tree().paused = false
	get_tree().change_scene_to_file("res://Scenes/MainMenu.tscn")

func _on_game_over(_Message):
	var sfxPlayer = localSoundManager.get_child(1) as AudioStreamPlayer
	var bgPlayer = localSoundManager.get_child(0) as AudioStreamPlayer
	sfxPlayer.play(0.0)
	bgPlayer.stop()
	totalScore.text = "SCORE : " + str(localGameManager.get("playerScore"))
	localSoundManager.process_mode = Node.PROCESS_MODE_WHEN_PAUSED
	self.process_mode = Node.PROCESS_MODE_WHEN_PAUSED
	pauseBt.hide()
	gameOverPanel.show()
	get_tree().paused = true
