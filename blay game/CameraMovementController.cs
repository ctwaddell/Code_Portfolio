using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CameraMovementController : MonoBehaviour
{
    public const float HEIGHT = 0.0f;
    public const float DISTANCE = 14.142f;
    public const float STARTTHETA = 45;
    public const float STARTPHI = 60;
    Vector3 OFFSET = new Vector3(2.0f, 0.0f, 2.0f);
    Vector3 FOCUS = new Vector3(0.0f, 0.6f, 0.0f);
    public float theta = 45; //starting offset, "front"
    public float phi = 60; //starting offtset, "down"
    public bool canMove = true;
    bool isAnimating = false;
    public CameraState state;
    KeyCode queueCode;
    Coroutine currentRoutine;

    #region References
        CameraHandler cameraHandler;
        EventHandler eventHandler;
        [SerializeField] AnimationCurve positionCurve;
        [SerializeField] AnimationCurve upCurve;
        [SerializeField] AnimationCurve downCurve;
        [SerializeField] AnimationCurve introCurve;
    #endregion

    void Awake()
    {
        FindReferences();

        theta = STARTTHETA;
        phi = STARTPHI;
        SetState(theta, phi);
    }

    void FindReferences()
    {
        cameraHandler = CameraHandler.GetInstance();
        eventHandler = EventHandler.GetInstance();
    }

    void Update()
    {
        if(Input.anyKeyDown && canMove) MoveCamera(); //pressing a key and can move
        else if(Input.anyKeyDown && !canMove) Enqueue();//pressing a key and cant move
        
        Debug.DrawRay(transform.position, transform.forward * 2, UnityEngine.Color.white);
        Debug.DrawRay(transform.position, new Vector3(transform.up.x * 2, 0, transform.up.z *2), UnityEngine.Color.red);
        Debug.DrawRay(transform.position, transform.up * 2, UnityEngine.Color.green);
    }

    //MOVEMENT LOGIC METHODS

    public void SetState()
    {
        SetState(STARTTHETA, STARTPHI);
    }

    public void SetState(float theta, float phi)
    {
        transform.position = Evaluate(theta, phi);
        transform.LookAt(FOCUS, Vector3.up);
    }

    public void MoveCamera()
    {
        if(Input.GetKeyDown(KeyCode.A) || Input.GetKeyDown(KeyCode.LeftArrow)) //Move LEFT
        {
            MoveLeft();
        }
        else if(Input.GetKeyDown(KeyCode.D) || Input.GetKeyDown(KeyCode.RightArrow)) //Move RIGHT
        {
            MoveRight();
        }
        else if(Input.GetKeyDown(KeyCode.W) || Input.GetKeyDown(KeyCode.UpArrow)) //Move UP
        {
            MoveUp();
        }
        else if(Input.GetKeyDown(KeyCode.S) || Input.GetKeyDown(KeyCode.DownArrow)) //Move DOWN
        {
            MoveDown();
        }
    }

    void QueueMoveCamera()
    {
        switch(queueCode)
        {
            case(KeyCode.A):
            case(KeyCode.LeftArrow):
                MoveLeft();
                break;
            case(KeyCode.D):
            case(KeyCode.RightArrow):
                MoveRight();
                break;
            case(KeyCode.W):
            case(KeyCode.UpArrow):
                MoveUp();
                break;
            case(KeyCode.S):
            case(KeyCode.DownArrow):
                MoveDown();
                break;
            default:
                break;
        }
        queueCode = KeyCode.None;
    }

    void Enqueue()
    {
        if(Input.GetKeyDown(KeyCode.A) || Input.GetKeyDown(KeyCode.LeftArrow)) //Move LEFT
        {
            queueCode = KeyCode.A;
        }
        else if(Input.GetKeyDown(KeyCode.D) || Input.GetKeyDown(KeyCode.RightArrow)) //Move RIGHT
        {
            queueCode = KeyCode.D;
        }
        else if(Input.GetKeyDown(KeyCode.W) || Input.GetKeyDown(KeyCode.UpArrow)) //Move UP
        {
            queueCode = KeyCode.W;
        }
        else if(Input.GetKeyDown(KeyCode.S) || Input.GetKeyDown(KeyCode.DownArrow)) //Move DOWN
        {
            queueCode = KeyCode.S;
        }
        else 
        {
            queueCode = KeyCode.None;
        }
    }

    void CheckQueue()
    {
        if(queueCode != KeyCode.None)
        {
            QueueMoveCamera();
        }
    }

    void MoveLeft()
    {
        if(state == CameraState.DOWN) currentRoutine = StartCoroutine(AnimateLateral(theta, theta - 90));
        else if (state == CameraState.UP) currentRoutine = StartCoroutine(AnimatePolar(theta, theta - 90));
    }

    void MoveRight()
    {
        if(state == CameraState.DOWN) currentRoutine = StartCoroutine(AnimateLateral(theta, theta + 90));
        else if (state == CameraState.UP) currentRoutine = StartCoroutine(AnimatePolar(theta, theta + 90));
    }

    void MoveUp()
    {
        if(state == CameraState.UP) return;
        currentRoutine = StartCoroutine(AnimateUp(theta));
    }

    void MoveDown()
    {
        if(state == CameraState.DOWN) return;
        currentRoutine = StartCoroutine(AnimateDown(theta));
    }

    //ANIMATION METHODS

    IEnumerator AnimateLateral(float originTheta, float destinationTheta)
    {
        canMove = false;
        eventHandler.InvokeCameraStartAnimate();

        float time = 0.0f;
        float DURATION = AnimationDuration.Lateral();
        while(time < DURATION)
        {
            float t = time / DURATION;
            theta = originTheta + ((destinationTheta - originTheta) * positionCurve.Evaluate(t));

            transform.position = Evaluate(theta, 60);
            transform.LookAt(FOCUS, Vector3.up);

            time += Time.deltaTime;
            yield return null;
        }

        transform.position = Evaluate(destinationTheta, 60);
        transform.LookAt(FOCUS, Vector3.up);

        theta = (((destinationTheta % 360) + 360) % 360);

        eventHandler.InvokeCameraStopAnimate();
        canMove = true;

        CheckQueue();
    }

    IEnumerator AnimatePolar(float originTheta, float destinationTheta)
    {
        canMove = false;
        eventHandler.InvokeCameraStartAnimate();

        destinationTheta = (((destinationTheta % 360) + 360) % 360);

        Vector3 originVec = ThetaToVector(originTheta);
        Vector3 destinationVec = ThetaToVector(destinationTheta);
        
        float time = 0.0f;
        float DURATION = AnimationDuration.Lateral();
        while(time < DURATION)
        {
            float t = time / DURATION;
            Vector3 upVec = Vector3.Lerp(originVec, destinationVec, positionCurve.Evaluate(t));

            transform.LookAt(FOCUS, upVec);

            time += Time.deltaTime;
            yield return null;
        }

        transform.LookAt(FOCUS, destinationVec);

        theta = destinationTheta;

        eventHandler.InvokeCameraStopAnimate();
        canMove = true;

        CheckQueue();
    }

    IEnumerator AnimateUp(float originTheta)
    {
        canMove = false;
        eventHandler.InvokeCameraStartAnimate();

        state = CameraState.UP;

        Vector3 originVec = Vector3.up;
        Vector3 destinationVec = ThetaToVector(originTheta);

        float originPhi = 60;
        float destinationPhi = 0;

        float time = 0.0f;
        float DURATION = AnimationDuration.Lateral();
        while(time < DURATION)
        {
            float t = time / DURATION;
            float currentPhi = originPhi + ((destinationPhi - originPhi) * positionCurve.Evaluate(t));

            Vector3 upVec = Vector3.Lerp(originVec, destinationVec, upCurve.Evaluate(t));

            transform.position = Evaluate(originTheta, currentPhi);
            transform.LookAt(FOCUS, upVec);

            time += Time.deltaTime;
            yield return null;
        }

        eventHandler.InvokeCameraStopAnimate();
        canMove = true;

        CheckQueue();
    }

    IEnumerator AnimateDown(float originTheta)
    {
        canMove = false;
        eventHandler.InvokeCameraStartAnimate();

        state = CameraState.DOWN;

        Vector3 originVec = ThetaToVector(originTheta);
        Vector3 destinationVec = Vector3.up;

        float originPhi = 0;
        float destinationPhi = 60;

        float time = 0.0f;
        float DURATION = AnimationDuration.Lateral();
        while(time < DURATION)
        {
            float t = time / DURATION;
            float currentPhi = originPhi + ((destinationPhi - originPhi) * positionCurve.Evaluate(t));

            Vector3 upVec = Vector3.Lerp(originVec, destinationVec, downCurve.Evaluate(t));

            transform.position = Evaluate(originTheta, currentPhi);
            transform.LookAt(FOCUS, upVec);

            time += Time.deltaTime;
            yield return null;
        }

        eventHandler.InvokeCameraStopAnimate();
        canMove = true;

        CheckQueue();
    }

    //HELPER METHODS

    //essentially a polar to cartesian converter
    Vector3 Evaluate(float theta, float phi)
    {
        float thetaRad = theta * Mathf.Deg2Rad;
        float phiRad = phi * Mathf.Deg2Rad;
        Vector3 returnVec = new Vector3
        (
            Mathf.Cos(thetaRad) * Mathf.Sin(phiRad), 
            Mathf.Cos(phiRad), 
            Mathf.Sin(thetaRad) * Mathf.Sin(phiRad)
        );

        returnVec *= DISTANCE;
        return returnVec;
    }

    Vector3 ThetaToVector(float theta)
    {
        switch(theta)
        {
            case(45):
                return new Vector3(-1, 0, 0);
            case(135):
                return new Vector3(0, 0, -1);
            case(225):
                return new Vector3(1, 0, 0);
            case(315):
                return new Vector3(0, 0, 1);
            default:
                return Vector3.up;
        }
    }

    public IEnumerator IntroAnimation()
    {
        canMove = false;

        theta = STARTTHETA;
        phi = STARTPHI;
        SetState(theta, phi);

        Vector3 startPos = new Vector3(transform.localPosition.x, transform.localPosition.y + 10.0f, transform.localPosition.z);
        Vector3 endPos = transform.position;

        transform.position = startPos;

        float time = 0.0f;
        float DURATION = AnimationDuration.CameraIntroMovement();
        while(time < DURATION)
        {
            float t = time / DURATION;
            Vector3 currentPos = Vector3.Lerp(startPos, endPos, introCurve.Evaluate(t));
            transform.position = currentPos;

            time += Time.deltaTime;
            yield return null;
        }

        transform.position = endPos;

        canMove = true;
    }

    public IEnumerator ExitAnimation()
    {
        if(state == CameraState.UP)
        {
            yield return AnimateDown(theta);
        }

        yield return cameraHandler.cameraRaycastController.ExitAnimation();

        canMove = false;

        Vector3 startPos = transform.localPosition;
        Vector3 endPos = new Vector3(transform.localPosition.x, transform.localPosition.y - 10.0f, transform.localPosition.z);

        transform.position = startPos;

        float time = 0.0f;
        float DURATION = AnimationDuration.CameraIntroMovement();
        while(time < DURATION)
        {
            float t = time / DURATION;
            Vector3 currentPos = Vector3.Lerp(startPos, endPos, introCurve.Evaluate(t));
            transform.position = currentPos;

            time += Time.deltaTime;
            yield return null;
        }

        transform.position = endPos;

        canMove = false;
    }
}


