extends Node

var gameClient = load("res://bin/gameClient.gdns").new()
func _ready():
	gameClient.InitGameClient(27016);

func _process(delta):
	gameClient.Update()
	var a = delta