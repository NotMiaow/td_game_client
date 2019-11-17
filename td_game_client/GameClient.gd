extends Node

var gameClient
func _ready():
	gameClient = load("res://bin/gameClient.gdns").new()
	gameClient.InitGameClient(27016);

#func _process(delta):
#	gameClient.Update()