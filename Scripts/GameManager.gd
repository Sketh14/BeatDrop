extends Node

var playerScore
@export var currentScore : Label
@export var localDropSpawner : DropsSpawner

# Called when the node enters the scene tree for the first time.
func _ready():
	playerScore = 0
	localDropSpawner.scene2Selected = GloballyAccessible.scene2Selected
	
	var scene
	if (!localDropSpawner.scene2Selected):
		scene = load("res://Scenes/BG2/BG_2.tscn")
#		print("Loading Scene1")
	else:
		scene = load("res://Scenes/BG_1.tscn")
#		print("Loading Scene2")
	var instance = scene.instantiate()
	add_child(instance)

func _on_drop_collected(_Message):
	playerScore = playerScore + 1;
	currentScore.text = "SCORE : " + str(playerScore)
#	print("Player Score : ", playerScore)
