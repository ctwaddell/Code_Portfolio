using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class LevelSelectView : MonoBehaviour, IPointerDownHandler, IPointerUpHandler
{
    public int currentLevel;
    public int numPuzzles;
    public bool isMoving;
    public int rightMarginWidth;
    public int buttonWidth;
    public int spacingWidth;

    Coroutine scrollCoroutine;
    public AnimationCurve moveToCurve;

    bool mouseDown;

    #region References
        [SerializeField] LevelSelectMenuManager levelSelectMenuManager;
        [SerializeField] RectTransform contentRectTransform;
        [SerializeField] ScrollRect scrollRect;
        [SerializeField] RectTransform viewRectTransform;
    #endregion

    void Update()
    {
        if(Input.GetMouseButtonDown(0))
        {
            if(RectTransformUtility.RectangleContainsScreenPoint(viewRectTransform, Input.mousePosition))
            {
                mouseDown = true;
            }
        }
        if(Input.GetMouseButtonUp(0) && mouseDown)
        {
            mouseDown = false;
            if(!isMoving) SnapTo();
        }
        if(Input.GetAxis("Mouse ScrollWheel") != 0.0f)
        {
            if(scrollCoroutine != null)
            {
                StopCoroutine(scrollCoroutine);
            }
            scrollCoroutine = StartCoroutine(Scroll());
        }
    }
    
    public void OnPointerDown(PointerEventData eventData)
    {
        mouseDown = true;
    }

    public void OnPointerUp(PointerEventData eventData)
    {
        if(!isMoving) SnapTo();
    }

    public int Recalculate()
    {
        currentLevel = (int)((Mathf.Abs(contentRectTransform.anchoredPosition.x) - rightMarginWidth - (spacingWidth / 2)) / (buttonWidth + spacingWidth)) + 1;
        if(currentLevel < 1) currentLevel = 1;
        if(currentLevel > numPuzzles) currentLevel = numPuzzles;
        return currentLevel;
    }

    public void NextLevel()
    {
        currentLevel++;
        if(currentLevel > numPuzzles) currentLevel = numPuzzles;
        if(!isMoving) SnapTo(currentLevel);
    }

    public void PrevLevel()
    {
        currentLevel--;
        if(currentLevel < 1) currentLevel = 1;
        if(!isMoving) SnapTo(currentLevel);
    }

    public void SnapTo()
    {
        scrollRect.StopMovement();
        StartCoroutine(MoveTo(new Vector2(-GetSnapPos(currentLevel), 150.0f)));
    }    

    public void SnapTo(int index)
    {
        scrollRect.StopMovement();
        StartCoroutine(MoveTo(new Vector2(-GetSnapPos(index), 150.0f)));
    }

    public void SnapToInstant(int index)
    {
        scrollRect.StopMovement();
        contentRectTransform.anchoredPosition = new Vector2(-GetSnapPos(index), 150.0f);
    }

    float GetSnapPos(int index)
    {
        return (((index) * (buttonWidth + spacingWidth)) + rightMarginWidth) - 100;
    }

    IEnumerator MoveTo(Vector2 targetPosition)
    {
        isMoving = true;
        Vector2 startPosition = contentRectTransform.anchoredPosition;

        float time = 0.0f;
        float DURATION = AnimationDuration.LevelSelectSnap();
        while(time < DURATION)
        {
            float t = time / DURATION;
            float curveValue = moveToCurve.Evaluate(t);
            contentRectTransform.anchoredPosition = Vector2.Lerp(startPosition, targetPosition, curveValue);
            time += Time.deltaTime;
            yield return null;
        }

        contentRectTransform.anchoredPosition = targetPosition;
        scrollRect.StopMovement();
        isMoving = false;
    }

    IEnumerator DelayedSnap(int index) //maybe useful one day
    {
        yield return null;

        SnapTo(index);
    }
    
    IEnumerator Scroll()
    {
        yield return new WaitForSeconds(AnimationDuration.LevelSelectScrollDelay());
        scrollCoroutine = null;
        SnapTo();
    }
}
