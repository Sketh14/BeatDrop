extends Node

######################## STREAK ############################
var streakPos = [Vector2(0.0, 0.0), Vector2(0.0, 0.0)]
var time_taken = 0.0
@export var streakSpeedMult : float = 1.0
@export var bg_Streak : Control

######################## CLOUDS ############################
#We don't need a vec2 array, because as the positions will stay constant with only the x-changing,
#we can just har-code y-pos
var fgCloudsInitPosX = [0.0, 0.0]
var bgCloudsInitPosX = [0.0, 0.0]
var tempPosX = 0.0

@export var fg_Clouds : Array[Control]
@export var bg_Clouds : Array[Control]
@export var moveSpeedMult : Array[float]

func _ready():
	streakPos[0] = Vector2(-1025.0, 92.0)
	streakPos[1] = Vector2(460.0, 143.0)
	
	for i in 2:
		fgCloudsInitPosX[i] = fg_Clouds[i].position.x;
		bgCloudsInitPosX[i] = bg_Clouds[i].position.x;
	
func _process(delta):
######################## STREAK ############################
	time_taken += delta * streakSpeedMult

	if (time_taken > 1):
		time_taken = 0

	bg_Streak.position = Vector2(lerp(streakPos[0], streakPos[1], time_taken))
######################## STREAK ############################
	
######################## CLOUDS ############################
	for i in 2:
		if (fgCloudsInitPosX[i] <= -1554.5):
			fgCloudsInitPosX[i] = 1554.5
		if (bgCloudsInitPosX[i] <= -1554.5):
			bgCloudsInitPosX[i] = 1554.5

		fgCloudsInitPosX[i] -= moveSpeedMult[i] * delta
		bgCloudsInitPosX[i] -= moveSpeedMult[i] * delta

		fg_Clouds[i].set_position(Vector2(fgCloudsInitPosX[i], 405.0))
		bg_Clouds[i].set_position(Vector2(bgCloudsInitPosX[i], 405.0))
#		print(bg_Clouds[0].position)
######################## CLOUDS ############################
