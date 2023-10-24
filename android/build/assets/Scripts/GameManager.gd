extends Node

var playerScore
@export var currentScore : Label

# Called when the node enters the scene tree for the first time.
func _ready():
	playerScore = 0

func _on_drop_collected(_Message):
	playerScore = playerScore + 1;
	currentScore.text = "SCORE : " + str(playerScore)
#	print("Player Score : ", playerScore)
