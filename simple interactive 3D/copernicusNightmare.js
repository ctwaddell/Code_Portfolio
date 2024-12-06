// JavaScript source code
function setup()
{
    //instantiation
    var canvas = document.getElementById('mainCanvas');
    var context = canvas.getContext('2d');

    var zRotation = 12000;
    var xRotation = 23550;

    document.addEventListener("keypress", function (event)
    {
        if (event.key === "w")
        {
            console.log(zRotation);
            zRotation = zRotation + 200;
            if (zRotation >= 31415)
            {
                zRotation = 31414;
            }
            render();
        }
        if (event.key === "s")
        {
            console.log(zRotation);
            zRotation = zRotation - 200;
            if (zRotation <= 0)
            {
                zRotation = 1;
            }
            render();
        }
        if (event.key === "a")
        {
            xRotation = xRotation - 200;
            render();
        }
        if (event.key === "d")
        {
            xRotation = xRotation + 200;
            render();
        }
    });

    var globalX;
    var globalY;
    var globalZ;

    var camPos;

    var pV1 = vec3.fromValues(100,100,100);
    var pV2 = vec3.fromValues(100,-100,100);
    var pV3 = vec3.fromValues(-100,-100,100);
    var pV4 = vec3.fromValues(-100,100,100);
    var pV5 = vec3.fromValues(100,100,-100);
    var pV6 = vec3.fromValues(100,-100,-100);
    var pV7 = vec3.fromValues(-100,100,-100);
    var pV8 = vec3.fromValues(-100,-100,-100);

    var planetCenter = vec3.fromValues(0, 0, 0);

    var sV1 = vec3.fromValues(20,20,20);
    var sV2 = vec3.fromValues(20,-20,20);
    var sV3 = vec3.fromValues(-20,-20,20);
    var sV4 = vec3.fromValues(-20,20,20);
    var sV5 = vec3.fromValues(20,20,-20);
    var sV6 = vec3.fromValues(20,-20,-20);
    var sV7 = vec3.fromValues(-20,20,-20);
    var sV8 = vec3.fromValues(-20, -20, -20);

    var sunCenter = vec3.fromValues(0, 0, 0);

    var mV1 = vec3.fromValues(40, 40, 40);
    var mV2 = vec3.fromValues(40, -40, 40);
    var mV3 = vec3.fromValues(-40, -40, 40);
    var mV4 = vec3.fromValues(-40, 40, 40);
    var mV5 = vec3.fromValues(40, 40, -40);
    var mV6 = vec3.fromValues(40, -40, -40);
    var mV7 = vec3.fromValues(-40, 40, -40);
    var mV8 = vec3.fromValues(-40, -40, -40);

    var moonCenter = vec3.fromValues(0, 0, 0);

    render();
 
    function render()
    {//matrices
        presentMatrix = mat4.create();
        baseCanvas = mat4.create();
        planetFromCanvas = mat4.create();
        sunFromPlanet = mat4.create();
        axesMatrix = mat4.create();
        moonFromPlanet = mat4.create();
        orthographic = mat4.create();
        preCam = mat4.create();

        var camPos = [(1000*Math.cos(.0001 * xRotation)) * Math.abs(Math.sin(.0001 * zRotation)),
                      (1000*Math.sin(.0001 * xRotation)) * Math.abs(Math.sin(.0001 * zRotation)),
                      (1000*Math.cos(.0001 * zRotation))];

        globalX = Math.sign(camPos[0]);
        globalY = Math.sign(camPos[1]);
        globalZ = Math.sign(camPos[2]);


        console.log("----------");
        console.log(camPos[0]);
        console.log(camPos[1]);
        console.log(camPos[2]);
        console.log("----------");
        console.log("==========");
        console.log(globalX);
        console.log(globalY);
        console.log(globalZ);
        console.log("==========");

        var camera = mat4.create();
        mat4.lookAt(camera, camPos, [0, 0, 0], [0, 0, -1]);

        var viewportCenter = mat4.create();
        mat4.fromTranslation(viewportCenter, [canvas.width/2, canvas.height/2, 0]);

        mat4.ortho(orthographic, -1, 1, -1, 1, -1, 1);
        mat4.multiply(preCam, viewportCenter, orthographic)
        mat4.multiply(camera, preCam, camera);

        mat4.multiply(baseCanvas, baseCanvas, camera);

        mat4.multiply(planetFromCanvas, planetFromCanvas, baseCanvas);

        mat4.rotateZ(sunFromPlanet, planetFromCanvas, 0);
        mat4.translate(sunFromPlanet, sunFromPlanet, [300, 300, 300]);

        mat4.rotateZ(moonFromPlanet, planetFromCanvas, 0);
        mat4.translate(moonFromPlanet, moonFromPlanet, [-400, -400, -400]);

        mat4.multiply(axesMatrix, axesMatrix, baseCanvas);

        vec3.set(planetCenter, 0, 0, 0);
        vec3.set(sunCenter, 0, 0, 0);

        vec3.transformMat4(planetCenter, planetCenter, planetFromCanvas);
        vec3.transformMat4(sunCenter, sunCenter, sunFromPlanet);
        vec3.transformMat4(moonCenter, moonCenter, moonFromPlanet);

        drawAll();

        function drawAll()
        {
            
            {
                var p0 = [300,300,300]; //sun
                var d0 = [-1000,1000,0];
                var p1 = [-400,-400,-400]; //moon
                var d1 = [1000, -1000, 0];

                var P0 = [p0, d0, p1, d1];
                var P1 = [p1, d1, p0, d0];
            }

            var CParse = function (t)
            {
                if (t > 1)
                {
                    return t;
                }
                if (t <= 1)
                {
                    return t - 1;
                }
            }

            var HermiteCurve = function (t)
            {
                return [(2 * t * t * t) - (3 * t * t) + 1,
                        (t * t * t) - (2 * t * t) + t,
                        (-2 * t * t * t) + (3 * t * t),
                        (t * t * t) - (t * t)];
            }

            var C0 = function (t)
            {
                return CubicCurve(HermiteCurve, P0, t);
            }

            var C1 = function (t)
            {
                return CubicCurve(HermiteCurve, P1, t);
            }

            function CubicCurve(basis, P, t)
            {
                var b = basis(t);
                var result = vec3.create();
                vec3.scale(result, P[0], b[0]); //b[0] is undefined...
                vec3.scaleAndAdd(result, result, P[1], b[1]);
                vec3.scaleAndAdd(result, result, P[2], b[2]);
                vec3.scaleAndAdd(result, result, P[3], b[3]);
                return result;
            }

            function drawTrajectory(tbegin, tend, intervals, C, matrix)
            {
                presentMatrix = matrix;
                context.strokeStyle = 'white';
                context.lineWidth = 4;
                context.beginPath();
                moveToTxV(C(tbegin));

                for (var i = 1; i <= intervals; i++)
                {
                    var t = ((intervals - i) / intervals) * tbegin + (i / intervals) * tend;
                    lineToTxV(C(t));
                }

                context.stroke();
            }

            canvas.width = canvas.width;
            drawSkyBox();



            console.log()

            if (vec3.distance(camPos, sunCenter) >= vec3.distance(camPos, planetCenter)) //sun is farther than planet
            {
                if (globalX < 0 && globalY > 0)
                {     
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                    drawSun();
                    drawPlanet();
                    drawMoon();
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                }
                if (globalX < 0 && globalY < 0)
                {
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                    drawSun();
                    drawPlanet();
                    drawMoon();
                }
                if (globalX > 0 && globalY > 0)
                {
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                    drawSun();
                    drawPlanet();
                    drawMoon();
                }
                if (globalX > 0 && globalY < 0)
                {
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                    drawSun();
                    drawPlanet();
                    drawMoon();
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                }
                
                return;
            }
            if (vec3.distance(camPos, sunCenter) < vec3.distance(camPos, planetCenter)) //sun is closer than planet
            {
                if (globalX < 0 && globalY > 0)
                {
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                    drawMoon();
                    drawPlanet();
                    drawSun();
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                }
                if (globalX < 0 && globalY < 0)
                {
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                    drawMoon();
                    drawPlanet();
                    drawSun();
                }
                if (globalX > 0 && globalY > 0)
                {
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                    drawMoon();
                    drawPlanet();
                    drawSun();
                }
                if (globalX > 0 && globalY < 0)
                {
                    drawTrajectory(0.0, 1.0, 100, C0, planetFromCanvas);
                    drawMoon();
                    drawPlanet();
                    drawSun();
                    drawTrajectory(0.0, 1.0, 100, C1, planetFromCanvas);
                }

                return;
            }
        }
    }

    function drawAxes()
    {
        presentMatrix = axesMatrix;

        context.lineWidth = 1;

        context.strokeStyle = 'red';
        context.beginPath();

        moveToTx(0, 0, 0);
        lineToTx(100, 0, 0);

        context.stroke();

        context.strokeStyle = 'lime';
        context.beginPath();

        moveToTx(0, 0, 0);
        lineToTx(0, 100, 0);

        context.stroke();

        context.strokeStyle = 'blue';
        context.beginPath();

        moveToTx(0, 0, 0);
        lineToTx(0, 0, 100);

        context.stroke();
    }

    function drawSun()
    {
        presentMatrix = sunFromPlanet;

        if (globalX >= 0 && globalY >= 0 && globalZ >= 0) //+++
        {
            drawFPlane();
            drawLPlane();
            drawTPlane();
            return;
        }

        if (globalX >= 0 && globalY >= 0 && globalZ < 0) //++-
        {
            drawFPlane();
            drawLPlane();
            drawBPlane();
            return;
        }

        if (globalX >= 0 && globalY < 0 && globalZ >= 0) //+-+
        {
            drawFPlane();
            drawRPlane();
            drawTPlane();
            return;
        }

        if (globalX >= 0 && globalY < 0 && globalZ < 0) //+--
        {
            drawFPlane();
            drawRPlane();
            drawBPlane();
            return;
        }

        if (globalX < 0 && globalY >= 0 && globalZ >= 0) //-++
        {
            drawXPlane();
            drawLPlane();
            drawTPlane();
            return;
        }

        if (globalX < 0 && globalY >= 0 && globalZ < 0) //-+-
        {
            drawXPlane();
            drawLPlane();
            drawBPlane();
            return;
        }

        if (globalX < 0 && globalY < 0 && globalZ >= 0) //--+
        {
            drawXPlane();
            drawRPlane();
            drawTPlane();
            return;
        }

        if (globalX < 0 && globalY < 0 && globalZ < 0) //---
        {
            drawXPlane();
            drawRPlane();
            drawBPlane();
            return;
        }
        if (globalY == 0 || globalY == -0)
        {
            console.log("error");
            globalY = globalY + .0001;
            drawTPlane();
        }

        function drawFPlane() //freeze xpos
        {
            context.strokeStyle = 'gold';
            context.lineWidth = 1;
            context.fillStyle = 'yellow';

            context.beginPath();
            moveToTxV(sV6);
            lineToTxV(sV2);
            lineToTxV(sV1);
            lineToTxV(sV5);
            lineToTxV(sV6);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawXPlane() //freeze xneg
        {
            context.strokeStyle = 'gold';
            context.lineWidth = 1;
            context.fillStyle = 'yellow';

            context.beginPath();
            moveToTxV(sV7);
            lineToTxV(sV4);
            lineToTxV(sV3);
            lineToTxV(sV8);
            lineToTxV(sV7);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawRPlane() //freeze yneg
        {
            context.strokeStyle = 'gold';
            context.lineWidth = 1;
            context.fillStyle = 'yellow';

            context.beginPath();
            moveToTxV(sV8);
            lineToTxV(sV3);
            lineToTxV(sV2);
            lineToTxV(sV6);
            lineToTxV(sV8);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawLPlane() //freeze ypos
        {
            context.strokeStyle = 'gold';
            context.lineWidth = 1;
            context.fillStyle = 'yellow';

            context.beginPath();
            moveToTxV(sV7);
            lineToTxV(sV4);
            lineToTxV(sV1);
            lineToTxV(sV5);
            lineToTxV(sV7);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawTPlane() //freeze zpos
        {
            context.strokeStyle = 'gold';
            context.lineWidth = 1;
            context.fillStyle = 'yellow';

            context.beginPath();
            moveToTxV(sV1);
            lineToTxV(sV4);
            lineToTxV(sV3);
            lineToTxV(sV2);
            lineToTxV(sV1);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawBPlane() //freeze zneg
        {
            context.strokeStyle = 'gold';
            context.lineWidth = 1;
            context.fillStyle = 'yellow';

            context.beginPath();
            moveToTxV(sV8);
            lineToTxV(sV7);
            lineToTxV(sV5);
            lineToTxV(sV6);
            lineToTxV(sV8);
            context.closePath();
            context.stroke();
            context.fill();
        }
    }

    function drawPlanet()
    {
        presentMatrix = planetFromCanvas;

        if (globalX >= 0 && globalY >= 0 && globalZ >= 0) //+++
        {
            drawFPlane();
            drawLPlane();
            drawTPlane();
            return;
        }

        if (globalX >= 0 && globalY >= 0 && globalZ < 0) //++-
        {
            drawFPlane();
            drawLPlane();
            drawBPlane();
            return;
        }

        if (globalX >= 0 && globalY < 0 && globalZ >= 0) //+-+
        {
            drawFPlane();
            drawRPlane();
            drawTPlane();
            return;
        }

        if (globalX >= 0 && globalY < 0 && globalZ < 0) //+--
        {
            drawFPlane();
            drawRPlane();
            drawBPlane();
            return;
        }

        if (globalX < 0 && globalY >= 0 && globalZ >= 0) //-++
        {
            drawXPlane();
            drawLPlane();
            drawTPlane();
            return;
        }

        if (globalX < 0 && globalY >= 0 && globalZ < 0) //-+-
        {
            drawXPlane();
            drawLPlane();
            drawBPlane();
            return;
        }

        if (globalX < 0 && globalY < 0 && globalZ >= 0) //--+
        {
            drawXPlane();
            drawRPlane();
            drawTPlane();
            return;
        }

        if (globalX < 0 && globalY < 0 && globalZ < 0) //---
        {
            drawXPlane();
            drawRPlane();
            drawBPlane();
            return;
        }
        if (globalY == 0 || globalY == -0)
        {
            console.log("error");
            globalY = globalY + .0001;
            drawTPlane();
        }

        function drawBPlane()
        {
            //bottomPlane freeze zneg
            context.strokeStyle = 'black';
            context.lineWidth = 1;
            context.fillStyle = '#00CBFF';

            context.beginPath();
            moveToTxV(pV8);
            lineToTxV(pV7);
            lineToTxV(pV5);
            lineToTxV(pV6);
            lineToTxV(pV8);
            context.closePath();
            context.stroke();
            context.fill();

            context.fillStyle = 'white';
            context.beginPath();
            moveToTx(-50, -50, -100);
            lineToTx(-50, 50, -100);
            lineToTx(50, 50, -100);
            lineToTx(50, -50, -100);
            lineToTx(-50, -50, -100);
            context.closePath();
            context.stroke();
            context.fill();
        }

        function drawLPlane()
        {
            //leftPlane freeze ypos
            context.strokeStyle = 'black';
            context.lineWidth = 1;
            context.fillStyle = '#00CBFF';

            context.beginPath();
            moveToTxV(pV7);
            lineToTxV(pV4);
            lineToTxV(pV1);
            lineToTxV(pV5);
            lineToTxV(pV7);
            context.closePath();
            context.stroke();
            context.fill();

            context.fillStyle = 'green';
            context.beginPath();

            moveToTx(-80,100,100);
            lineToTx(-60, 100, 100);
            lineToTx(-60, 100, 80);
            lineToTx(-80, 100,80);
            lineToTx(-80, 100,100);
            context.closePath();
            context.stroke();
            context.fill();

            context.beginPath();
            moveToTx(-100, 100,60);
            lineToTx(-40, 100, 60);
            lineToTx(-40, 100, 100);
            lineToTx(100, 100, 100);
            lineToTx(100, 100,-40);
            lineToTx(80, 100, -40);
            lineToTx(80, 100,0);
            lineToTx(20, 100,0);
            lineToTx(20, 100,40);
            lineToTx(-80, 100,40);
            lineToTx(-80, 100,20);
            lineToTx(-100, 100,20);
            lineToTx(-100, 100,60);
            context.closePath();
            context.stroke();
            context.fill();

            context.beginPath();
            moveToTx(0, 100, -100);
            lineToTx(-40, 100, -100);
            lineToTx(-40, 100, -40);
            lineToTx(-80, 100, -40);
            lineToTx(-80, 100,0);
            lineToTx(-60, 100,0);
            lineToTx(-60, 100,20);
            lineToTx(0, 100,20);
            lineToTx(0, 100,0);
            lineToTx(20, 100,0);
            lineToTx(20, 100,-60);
            lineToTx(0, 100, -60);
            lineToTx(0, 100,-100);
            context.closePath();
            context.stroke();
            context.fill();

        }

        function drawRPlane()
        {
            //rightPlane freeze yneg
            context.strokeStyle = 'black';
            context.lineWidth = 1;
            context.fillStyle = '#00CBFF';

            context.beginPath();
            moveToTxV(pV8);
            lineToTxV(pV3);
            lineToTxV(pV2);
            lineToTxV(pV6);
            lineToTxV(pV8);
            context.closePath();
            context.stroke();
            context.fill();

            context.fillStyle = 'green';
            context.beginPath();
            moveToTx(-40, -100, -40);
            lineToTx(-40, -100,-20);
            lineToTx(-20, -100,-20);
            lineToTx(-20, -100, -40);
            lineToTx(-40, -100, -40);
            context.closePath();
            context.fill();
            context.stroke();

            context.beginPath();
            moveToTx(-20, -100, -20);
            lineToTx(-20, -100,0);
            lineToTx(0, -100,0);
            lineToTx(0, -100, -20);
            lineToTx(-20, -100, -20);
            context.closePath();
            context.fill();
            context.stroke();
        }

        function drawFPlane()
        {
            //frontPlane freeze xpos
            context.strokeStyle = 'black';
            context.lineWidth = 1;
            context.fillStyle = '#00CBFF';

            context.beginPath();
            moveToTxV(pV6);
            lineToTxV(pV2);
            lineToTxV(pV1);
            lineToTxV(pV5);
            lineToTxV(pV6);
            context.closePath();
            context.stroke();
            context.fill();


            context.fillStyle = 'green';
            context.beginPath();
            moveToTx(100,100,100)
            lineToTx(100,100 ,-20);
            lineToTx(100,80 ,-20);
            lineToTx(100,80 ,20);
            lineToTx(100,40 ,20);
            lineToTx(100,40 ,-20);
            lineToTx(100,20 ,-20);
            lineToTx(100,20 ,-40);
            lineToTx(100,0 ,-40);
            lineToTx(100,0 ,0);
            lineToTx(100,-20 ,0);
            lineToTx(100, -20,40);
            lineToTx(100,-40 ,40);
            lineToTx(100, -40,80);
            lineToTx(100, -60,80);
            lineToTx(100, -60,20);
            lineToTx(100, -80, 20);
            lineToTx(100,-80 ,100);
            lineToTx(100, 100, 100);
            context.closePath();
            context.stroke();
            context.fill();

            context.beginPath();
            moveToTx(100,-20,-100);
            lineToTx(100,-20,-60);
            lineToTx(100,-40,-60);
            lineToTx(100,-40,-40);
            lineToTx(100,-80,-40);
            lineToTx(100,-80,-60);
            lineToTx(100,-100,-60);
            lineToTx(100,-100,-100);
            lineToTx(100,-60,-100);
            lineToTx(100,-60,-80);
            lineToTx(100,-40,-80);
            lineToTx(100,-40,-100);
            lineToTx(100,-20,-100);
            context.closePath();
            context.stroke();
            context.fill();

        }

        function drawXPlane()
        {
            //backPlane freeze xneg
            context.strokeStyle = 'black';
            context.lineWidth = 1;
            context.fillStyle = '#00CBFF';

            context.beginPath();
            moveToTxV(pV7);
            lineToTxV(pV4);
            lineToTxV(pV3);
            lineToTxV(pV8);
            lineToTxV(pV7);
            context.closePath();
            context.stroke();
            context.fill();

            context.fillStyle = 'green';
            context.beginPath();
            moveToTx(-100, 20, -100);
            lineToTx(-100, 20, -40);
            lineToTx(-100, 0, -40);
            lineToTx(-100, 0, 0);
            lineToTx(-100, -40, 0);
            lineToTx(-100, -40, 40);
            lineToTx(-100, -60, 40);
            lineToTx(-100, -60, 80);
            lineToTx(-100, -80, 80);
            lineToTx(-100, -80, 100);
            lineToTx(-100, 80, 100);
            lineToTx(-100, 80, 80);
            lineToTx(-100, 60, 80);
            lineToTx(-100, 60, 20);
            lineToTx(-100, 40, 20);
            lineToTx(-100, 40, 40);
            lineToTx(-100, -20, 40);
            lineToTx(-100, -20, 20);
            lineToTx(-100, 20, 20);
            lineToTx(-100, 20, 0);
            lineToTx(-100, 60, 0);
            lineToTx(-100, 60, -60);
            lineToTx(-100, 40, -60);
            lineToTx(-100, 40, -100);
            lineToTx(-100, 20, -100);
            context.closePath();
            context.stroke();
            context.fill();
        }

        function drawTPlane()
        {
            //topPlane freeze zpos
            context.strokeStyle = 'black';
            context.lineWidth = 1;
            context.fillStyle = '#00CBFF';

            context.beginPath();
            moveToTxV(pV1);
            lineToTxV(pV4);
            lineToTxV(pV3);
            lineToTxV(pV2);
            lineToTxV(pV1);
            context.closePath();
            context.stroke();
            context.fill();

            context.fillStyle = 'white';
            context.beginPath();
            moveToTx(-50, -50, 100);
            lineToTx(-50, 50, 100);
            lineToTx(50, 50, 100);
            lineToTx(50, -50, 100);
            lineToTx(-50, -50, 100);
            context.closePath();
            context.stroke();
            context.fill();
        }
    }

    function drawMoon()
    {
        presentMatrix = moonFromPlanet;

        if (globalX >= 0 && globalY >= 0 && globalZ >= 0) //+++
        {
            drawFPlane();
            drawLPlane();
            drawTPlane();
            return;
        }

        if (globalX >= 0 && globalY >= 0 && globalZ < 0) //++-
        {
            drawFPlane();
            drawLPlane();
            drawBPlane();
            return;
        }

        if (globalX >= 0 && globalY < 0 && globalZ >= 0) //+-+
        {
            drawFPlane();
            drawRPlane();
            drawTPlane();
            return;
        }

        if (globalX >= 0 && globalY < 0 && globalZ < 0) //+--
        {
            drawFPlane();
            drawRPlane();
            drawBPlane();
            return;
        }

        if (globalX < 0 && globalY >= 0 && globalZ >= 0) //-++
        {
            drawXPlane();
            drawLPlane();
            drawTPlane();
            return;
        }

        if (globalX < 0 && globalY >= 0 && globalZ < 0) //-+-
        {
            drawXPlane();
            drawLPlane();
            drawBPlane();
            return;
        }

        if (globalX < 0 && globalY < 0 && globalZ >= 0) //--+
        {
            drawXPlane();
            drawRPlane();
            drawTPlane();
            return;
        }

        if (globalX < 0 && globalY < 0 && globalZ < 0) //---
        {
            drawXPlane();
            drawRPlane();
            drawBPlane();
            return;
        }
        if (globalY == 0 || globalY == -0)
        {
            console.log("error");
            globalY = globalY + .0001;
            drawTPlane();
        }

        function drawFPlane() //freeze xpos
        {
            context.strokeStyle = 'gray';
            context.lineWidth = 1;
            context.fillStyle = '#D3D3D3';
            console.log("poog");
            context.beginPath();
            moveToTxV(mV6);
            lineToTxV(mV2);
            lineToTxV(mV1);
            lineToTxV(mV5);
            lineToTxV(mV6);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawXPlane() //freeze xneg
        {
            context.strokeStyle = 'gray';
            context.lineWidth = 1;
            context.fillStyle = '#D3D3D3';

            context.beginPath();
            moveToTxV(mV7);
            lineToTxV(mV4);
            lineToTxV(mV3);
            lineToTxV(mV8);
            lineToTxV(mV7);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawRPlane() //freeze yneg
        {
            context.strokeStyle = 'gray';
            context.lineWidth = 1;
            context.fillStyle = '#D3D3D3';

            context.beginPath();
            moveToTxV(mV8);
            lineToTxV(mV3);
            lineToTxV(mV2);
            lineToTxV(mV6);
            lineToTxV(mV8);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawLPlane() //freeze ypos
        {
            context.strokeStyle = 'gray';
            context.lineWidth = 1;
            context.fillStyle = '#D3D3D3';

            context.beginPath();
            moveToTxV(mV7);
            lineToTxV(mV4);
            lineToTxV(mV1);
            lineToTxV(mV5);
            lineToTxV(mV7);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawTPlane() //freeze zpos
        {
            context.strokeStyle = 'gray';
            context.lineWidth = 1;
            context.fillStyle = '#D3D3D3';

            context.beginPath();
            moveToTxV(mV1);
            lineToTxV(mV4);
            lineToTxV(mV3);
            lineToTxV(mV2);
            lineToTxV(mV1);
            context.closePath();
            context.stroke();
            context.fill();
        }
        function drawBPlane() //freeze zneg
        {
            context.strokeStyle = 'gray';
            context.lineWidth = 1;
            context.fillStyle = '#D3D3D3';

            context.beginPath();
            moveToTxV(mV8);
            lineToTxV(mV7);
            lineToTxV(mV5);
            lineToTxV(mV6);
            lineToTxV(mV8);
            context.closePath();
            context.stroke();
            context.fill();
        }
    }

    function drawSkyBox()
    {
        presentMatrix = baseCanvas;

        context.fillStyle = 'black';
        context.lineWidth = 5;
        context.strokeStyle = 'black';

        //bottomPlane freeze zneg
        context.beginPath();
        moveToTx(-100000, -100000, -100000);
        lineToTx(-100000, 100000, -100000);
        lineToTx(100000, 100000, -100000);
        lineToTx(100000, -100000, -100000);
        lineToTx(-100000, -100000, -100000);
        context.closePath();
        context.stroke();
        context.fill();

        //backPlane freeze ypos
        context.beginPath();
        moveToTx(-100000, 100000, -100000);
        lineToTx(-100000,100000,100000);
        lineToTx(100000,100000,100000);
        lineToTx(100000,100000,-100000);
        lineToTx(-100000,100000,-100000);
        context.closePath();
        context.stroke();
        context.fill();

        //frontPlane freeze yneg
        context.beginPath();
        moveToTx(-100000,-100000,-100000);
        lineToTx(-100000,-100000,100000);
        lineToTx(100000,-100000,100000);
        lineToTx(100000,-100000,-100000);
        lineToTx(-100000,-100000,-100000);
        context.closePath();
        context.stroke();
        context.fill();

        //leftPlane freeze xpos
        context.beginPath();
        moveToTx(100000,-100000,-100000);
        lineToTx(100000,-100000,100000);
        lineToTx(100000,100000,100000);
        lineToTx(100000,-100000,100000);
        lineToTx(100000,-100000,-100000);
        context.closePath();
        context.stroke();
        context.fill();

        //rightPlane freeze xneg
        context.beginPath();
        moveToTx(-100000,100000,-100000);
        lineToTx(-100000,100000,100000);
        lineToTx(-100000,-100000,100000);
        lineToTx(-100000,-100000,-100000);
        lineToTx(-100000,100000,-100000);
        context.closePath();
        context.stroke();
        context.fill();

        //topPlane freeze zpos
        context.beginPath();
        moveToTx(100000,100000,100000);
        lineToTx(-100000,100000,100000);
        lineToTx(-100000,-100000,100000);
        lineToTx(100000,-100000,100000);
        lineToTx(100000,100000,100000);
        context.closePath();
        context.stroke();
        context.fill();
    }

    function lineToTx(x, y, z)
    {
        var vector = vec3.create();
        vec3.transformMat4(vector, [x, y, z], presentMatrix);
        context.lineTo(vector[0], vector[1]);
    }

    function lineToTxV(pos)
    {
        var vector = vec3.create();
        vec3.transformMat4(vector, [pos[0], pos[1], pos[2]], presentMatrix);
        context.lineTo(vector[0], vector[1]);
    }

    function moveToTx(x, y, z)
    {
        var vector = vec3.create();
        vec3.transformMat4(vector, [x, y, z], presentMatrix);
        context.moveTo(vector[0], vector[1]);
    }

    function moveToTxV(pos)
    {
        var vector = vec3.create();
        vec3.transformMat4(vector, [pos[0], pos[1], pos[2]], presentMatrix);
        context.moveTo(vector[0], vector[1]);
    }

    
}
window.onload = setup();

