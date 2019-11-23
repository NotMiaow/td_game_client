extends Camera

var gameClient = load("res://bin/gameClient.gdns").new()

func _ready():
	gameClient.InitGameClient(27016, self.get_parent());
	
func _physics_process(delta):
	var space_state = get_world().direct_space_state
	var mousePos = get_viewport().get_mouse_position()
	var origin = self.project_ray_origin(mousePos)
	var worldPos = origin + project_ray_normal(mousePos) * 20
	var hit = space_state.intersect_ray(origin, worldPos)
	var mouseToWorldPos = null
	if hit:
		mouseToWorldPos = Vector2(hit.position.x, hit.position.z)
	gameClient.Update(delta, mouseToWorldPos)
	var a = delta