extends Node

var gameClient
func _ready():
	gameClient = load("res://bin/gameClient.gdns").new()

func _process(delta):
	gameClient.Iteration()