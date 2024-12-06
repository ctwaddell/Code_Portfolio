using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Dialogue : MonoBehaviour
{
    public string textToDisplay;

    public float inputTextSpeed;

    public GameObject dialogueHandler;

    public AudioSource dialogueNoise;

    float tempTextSpeed;

    float realTextSpeed;

    char[] charArray;

    int charIndex = 0;

    Text toPrint;

    bool printing;

    bool speedUp;

    private void Start()
    {
        tempTextSpeed = inputTextSpeed;
    }

    private void Awake()
    {
        dialogueHandler = GameObject.Find("DialogueHandler");
        printing = true;
    }

    private void Update()
    {
        speedUp = false;
        if (printing == false)
        {
            if (Input.GetMouseButtonDown(0) == true || Input.anyKeyDown == true) //If the text is stopped, resume
            {
                printing = true;
                return;
            }
        }
        if (Input.GetMouseButton(0) == true ||Input.anyKey == true) //If the user is cinputting, speed up the text speed
        {
            speedUp = true;
            realTextSpeed = 0.005f;
            return;
        }
        
        realTextSpeed = tempTextSpeed;
    }

    /// <summary>
    /// Reads in Text component and starts PrintCoroutine
    /// </summary>
    public void Print()
    {
        toPrint = GetComponent<Text>();
        toPrint.text = "";
        StartCoroutine("PrintCoroutine");
    }

    /// <summary>
    /// Starts PrintCoroutine using an input text string
    /// </summary>
    /// <param name="textToDisplayInput"> String of what text you wish to print</param>
    public void Print(string textToDisplayInput)
    {
        textToDisplay = textToDisplayInput;
        charArray = textToDisplay.ToCharArray();
        toPrint = GetComponent<Text>();
        toPrint.text = "";
        StartCoroutine("PrintCoroutine");
    }

    /// <summary>
    /// Stops PrintCoroutine
    /// </summary>
    public void StopPrint()
    {
        StopCoroutine("PrintCoroutine");
    }

    /// <summary>
    /// Deals with the actual interpreting and printing of the text input
    /// </summary>
    IEnumerator PrintCoroutine()
    {
        printing = true;
        int i = charIndex;
        dialogueNoise.loop = true;
        dialogueNoise.Play(0);
        for (i = 0; i < charArray.Length; i++)
        {
            charIndex = i;
            if (charArray[i].Equals('\\')) //a backslash followed by...
            {
                if (charArray[i + 1].Equals('\\')) //a backslash indicates a break in dialogue
                {
                    dialogueNoise.loop = false;
                    i++;
                    charIndex = i;
                    IndicatePause();
                    printing = false;
                    while (printing == false) yield return null;
                    StopIndicatePause();
                    yield return new WaitForSeconds(0.25f);
                    Clear();
                    dialogueNoise.loop = true;
                    dialogueNoise.Play(0);
                    continue;
                }
                if (charArray[i + 1].Equals('o')) //an o indicates switching to speaker 0
                {
                    i++;
                    UpdateSpeaker(0);
                    continue;
                }
                if (charArray[i + 1].Equals('t')) //a t indicates switching to speaker 1
                {
                    i++;
                    UpdateSpeaker(1);
                    continue;
                }
                if (charArray[i + 1].Equals('h')) //an h indicates switching to speaker 2
                {
                    i++;
                    UpdateSpeaker(2);
                    continue;
                }
                if (charArray[i + 1].Equals('e')) //an e indicates a newline in the text
                {
                    i++;
                    toPrint.text += "\n";
                    continue;
                }
                //PORTRAIT KEY-
                //normal - 0
                //angry - 1
                //happy - 2
                //sad - 3
                //surprised - 4
                //inquisitive - 5
                //depressed - 6
                if (charArray[i + 1].Equals('0')) //a 0 indicates normal portrait
                {
                    i++;
                    UpdatePortrait("normal");
                    continue;
                }
                if (charArray[i + 1].Equals('1')) //a 1 indicates switch to angry portrait
                {
                    i++;
                    UpdatePortrait("angry");
                    continue;
                }
                if (charArray[i + 1].Equals('2')) //a 2 indicates switch to happy portrait
                {
                    i++;
                    UpdatePortrait("happy");
                    continue;
                }
                if (charArray[i + 1].Equals('3')) //a 3 indicates switch to sad portrait
                {
                    i++;
                    UpdatePortrait("sad");
                    continue;
                }
                if (charArray[i + 1].Equals('4')) //a 4 indicates switch to surprised portrait
                {
                    i++;
                    UpdatePortrait("surprised");
                    continue;
                }
                if (charArray[i + 1].Equals('5')) //a 5 indicates switch to inquisitive portrait
                {
                    i++;
                    UpdatePortrait("inquisitive");
                    continue;
                }
                if (charArray[i + 1].Equals('6')) //a 6 indicates switch to depressed portrait
                {
                    i++;
                    UpdatePortrait("depressed");
                    continue;
                }
                if (charArray[i + 1].Equals('p')) //a p indicates a long pause in dialogue
                {
                    if (speedUp == true)
                    {
                        i++;
                        dialogueNoise.loop = false;
                        yield return new WaitForSeconds(realTextSpeed);
                        dialogueNoise.loop = true;
                        dialogueNoise.Play(0);
                        continue;
                    }
                    else 
                    {
                        i++;
                        dialogueNoise.loop = false;
                        yield return new WaitForSeconds(2.0f);
                        dialogueNoise.loop = true;
                        dialogueNoise.Play(0);
                        continue;
                    }
                }
                if (charArray[i + 1].Equals('m')) //an m indicates a medium pause in dialogue
                {
                    if (speedUp == true)
                    {
                        i++;
                        dialogueNoise.loop = false;
                        yield return new WaitForSeconds(realTextSpeed);
                        dialogueNoise.loop = true;
                        dialogueNoise.Play(0);
                        continue;
                    }
                    else
                    {
                        i++;
                        dialogueNoise.loop = false;
                        yield return new WaitForSeconds(1.0f);
                        dialogueNoise.loop = true;
                        dialogueNoise.Play(0);
                        continue;
                    }
                }
                if (charArray[i + 1].Equals('s')) //an s indicates a short pause in dialogue
                {
                    if (speedUp == true)
                    {
                        i++;
                        dialogueNoise.loop = false;
                        yield return new WaitForSeconds(realTextSpeed);
                        dialogueNoise.loop = true;
                        dialogueNoise.Play(0);
                        continue;
                    }
                    else
                    {
                        i++;
                        dialogueNoise.loop = false;
                        yield return new WaitForSeconds(0.5f);
                        dialogueNoise.loop = true;
                        dialogueNoise.Play(0);
                        continue;
                    }
                }
                if (charArray[i + 1].Equals('.')) //a . indicates a slow ellipses in dialogue
                {
                    dialogueNoise.loop = false;
                    yield return new WaitForSeconds(0.1f);
                    if (speedUp == true)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (j == 0)
                            {
                                toPrint.text += "";
                            }
                            else
                            {
                                dialogueNoise.Play();
                                toPrint.text += '.';
                                toPrint.text += ' ';
                                yield return new WaitForSeconds(realTextSpeed);
                            }
                        }
                        dialogueNoise.loop = true;
                        dialogueNoise.Play();
                        i++;
                        continue;
                    }
                    else 
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (j == 0)
                            {
                                toPrint.text += "";
                            }
                            else
                            {
                                dialogueNoise.Play();
                                toPrint.text += '.';
                                toPrint.text += ' ';
                                yield return new WaitForSeconds(0.3f);
                            }
                        }
                        dialogueNoise.loop = true;
                        dialogueNoise.Play();
                        i++;
                        continue;
                    }
                }
                if (charArray[i + 1].Equals('r')) //an r indicates a clear in dialogue
                {
                    toPrint.text = "";
                    i++;
                    continue;
                }
                if (charArray[i + 1].Equals('!')) //an ! indicates the dialogue is finished
                {
                    dialogueNoise.loop = false;
                    IndicatePause();
                    printing = false;
                    while (printing == false) yield return null;
                    StopIndicatePause();
                    EndDialogue();
                    StopCoroutine("PrintCoroutine");
                    yield return new WaitForSeconds(0.0f);
                }
            }
            else
            {
                toPrint.text += charArray[i];
                yield return new WaitForSeconds(realTextSpeed);
            }
        }
    }


    //--------------------------------------------------
	//  COROUTINE METHODS -- FOR USE BY COROUTINE ONLY
	//--------------------------------------------------

    /// <summary>
    /// Clears the textbox
    /// </summary>
    public void Clear()
    {
        toPrint = GetComponent<Text>();
        toPrint.text = "";
    }

    /// <summary>
    /// Calls DialogueHandler's UpdateSpeaker method with requested speaker
    /// </summary>
    /// <param name="speakerNumber"> Local speakernumber to be switched to</param>
    public void UpdateSpeaker(int speakerNumber)
    {
        dialogueHandler.GetComponent<DialogueHandler>().UpdateSpeaker(speakerNumber);
    }

    /// <summary>
    /// Calls DialogueHandler's UpdatePortrait method with requested emotion (of current speaker)
    /// </summary>
    /// <param name="portraitSuffix"> Emotion of the current speaker to display</param>
    public void UpdatePortrait(string portraitSuffix)
    {
        dialogueHandler.GetComponent<DialogueHandler>().UpdatePortrait(portraitSuffix);
    }

    /// <summary>
    /// Desummons the DialogueBox
    /// </summary>
    public void EndDialogue()
    {
        dialogueHandler.GetComponent<DialogueHandler>().DesummonDialogueBox();
    }

    /// <summary>
    /// Pauses the dialogue, requires user input to resume dialogue
    /// </summary>
    public void IndicatePause()
    {
        dialogueHandler.GetComponent<DialogueHandler>().IndicatePause();
    }

    /// <summary>
    /// Resumes dialogue
    /// </summary>
    public void StopIndicatePause()
    {
        dialogueHandler.GetComponent<DialogueHandler>().StopIndicatePause();
    }

}
