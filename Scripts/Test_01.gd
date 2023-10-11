extends Node


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass



func _on_player_body_entered(body):
	print("Collision Detected")


func _on_rain_drop_area_entered(area):
	print("Collision Detected Area")


func _on_rain_drop_body_entered(body):
	print("Collision Detected Body Rain")


func _on_area_2d_body_entered(body):
	print("Collision Detected Area in side player")


func _on_player_touch_pressed():
	print("_on_player_touch_pressedr")
