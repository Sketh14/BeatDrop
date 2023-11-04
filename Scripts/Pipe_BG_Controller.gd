extends Control

@export var steam : Array[Control]
var animationPlayer = [0,0,0,0,0]
var steamTimeMult = [0.0, 0.0, 0.0, 0.0, 0.0] #: Array[float] #@export 
var time_taken = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
var gauge_time_taken = [0.0, 0.0]

#var leverTimeMult = [0.0, 0.0] # : Array[float] #@export
var leverSelected = [false, false, false, false]
@export var lever_312 : Control
@export var handle_278 : Control
@export var gauge_359_Needle_e : Control
@export var gauge_245_Needle_e : Control
#@export var gaugeNeedleRotDif : float
@export var needleAnimationPlayer : AnimationPlayer

# Called when the node enters the scene tree for the first time.
func _ready():
	for i in 5:
		animationPlayer[i] = steam[i].get_child(1).get_child(0) as AnimationPlayer
		steamTimeMult[i] = randf_range(0.2, .6)
		animationPlayer[i].play("Stweam")
#	for i in 2:
#		leverTimeMult[i] = randf_range(0.15, .35)
	callLeverTurnAfterRandomTime(1)
	callLeverTurnAfterRandomTime(2)
	needleAnimationPlayer.play("NeedleFlick")

#	print(steam[0].get_child(1).get_child(0).name)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for i in 5:
		time_taken[i] += delta * steamTimeMult[i]
#	for i in 2:
#		time_taken[i + 5] += delta * leverTimeMult[i]
#	time_taken[5] += delta * steamTimeMult[i]

	for i in 5:
		# Steam Animation
		if (time_taken[i] > 1):
			time_taken[i] = 0
#			for j in 5:
			animationPlayer[i].play("Stweam")
		#Lever Animation
#		if (i < 3 && time_taken[i + 4] > 1):
#			time_taken[i + 4] = 0
#			turnLever(i)
	
#	gaugeNeedleRotDif *= -1.0;
#	gauge_245_Needle_e.rotation += gaugeNeedleRotDif
#	gauge_359_Needle_e.rotation = lerpf(gaugeNeedleRotDif, gaugeNeedleRotDif * -1.0, delta)

func callLeverTurnAfterRandomTime(index):
	var timePassed = 0.0
	var randonTimeMult = randf_range(0.002, 0.003)
	while (true):
		await get_tree().create_timer(randonTimeMult).timeout
		timePassed += randonTimeMult
		if (timePassed > 1):
			turnLever(index)
			break

func turnLever(index):
	var selectedNode = lever_312
	match(index):
		1:
			selectedNode = lever_312
		2:
			selectedNode = handle_278
	
	var tempTime = 0.0
	var currentRot = selectedNode.rotation
#	print("Current Rot : ", currentRot)
	var finalRot =  180.0 if (currentRot <= 1.0) else 0.0
	while (true):
		await get_tree().create_timer(0.1).timeout
		tempTime += 0.1
		if (tempTime > 1):
			callLeverTurnAfterRandomTime(index)
			break
		selectedNode.rotation = lerpf(currentRot, finalRot, tempTime)
