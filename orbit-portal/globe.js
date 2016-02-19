var ps, acorn;

// Create an orbit camera halfway between the closest and farthest point
var cam = new OrbitCam({closest:5, farthest:40, distance: 20});
var isDragging = false;
var rotationStartCoords = [0, 0];

function zoom(amt){
    if(amt < 0){
	cam.goCloser(-amt);
    }
    else{
	cam.goFarther(amt);
    }
}

function mousePressed(){
    rotationStartCoords[0] = ps.mouseX;
    rotationStartCoords[1] = ps.mouseY;

    isDragging = true;
}

function mouseReleased(){
    isDragging = false;
}

function keyDown(){
    cam.setPosition([0, 0, 0]);
}

function render(){
    if(isDragging === true){
	// how much was the cursor moved compared to last time
	// this function was called?
	var deltaX = ps.mouseX - rotationStartCoords[0];
	var deltaY = ps.mouseY - rotationStartCoords[1];

	// now that the camera was updated, reset where the
	// rotation will start for the next time this function is called.
	rotationStartCoords = [ps.mouseX, ps.mouseY];

	cam.yaw(-deltaX * 0.015);
	cam.pitch(deltaY * 0.015);
    }

    cam.yaw(0.015);
    
    var c = acorn.getCenter();
    ps.multMatrix(M4x4.makeLookAt(cam.position, cam.direction, cam.up));
    ps.translate(-cam.position[0]-c[0], -cam.position[1]-c[1], -cam.position[2]-c[2] );

    ps.clear();
    ps.render(acorn);
}

function start(){
    ps = new PointStream();

    ps.setup(document.getElementById('canvas'));

    ps.background([0, 0, 0, 0.5]);
    ps.pointSize(5);

    ps.onRender = render;
    ps.onMouseScroll = zoom;
    ps.onMousePressed = mousePressed;
    ps.onMouseReleased = mouseReleased;
    ps.onKeyDown = keyDown;

    acorn = ps.load("clouds/out.asc");
    cam.pitch(0.3);
}
