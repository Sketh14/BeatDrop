extends Control

@export var steam : Array[Control]
var animationPlayer = [0,0,0,0,0]
@export var steamTimeMult : Array[float]
var time_taken = [0.0, 0.0, 0.0, 0.0, 0.0]

# Called when the node enters the scene tree for the first time.
func _ready():
	for i in 5:
		animationPlayer[i] = steam[i].get_child(1).get_child(0) as AnimationPlayer
		steamTimeMult[i] = randf_range(0.2, .6)
		animationPlayer[i].play("Stweam")
#	print(steam[0].get_child(1).get_child(0).name)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for i in 5:
		time_taken[i] += delta * steamTimeMult[i]

	for i in 5:
		if (time_taken[i] > 1):
			time_taken[i] = 0
#			for j in 5:
			animationPlayer[i].play("Stweam")
