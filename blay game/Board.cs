using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.SceneManagement;

public class Board : MonoBehaviour
{
    public string boardString; //the active state of the board, updated each move
    public string stateString; //the cached state of the board, savegame stuff
    public string solutionString; //the correct state of the board to win
    public string baseString; //the base state of the board for first creation

    public const float HEIGHTOFFSET = 0.0f; //allows individual control, but in reality just adjust the Board Object's Y please

    public int rows = 0;
    public int columns = 0;
    public GameObject[,] tiles;
    public int moves;

    #region References
        PuzzleHandler puzzleHandler;
        UIHandler uiHandler;
        StatsHandler statsHandler;
        EventHandler eventHandler;

        [SerializeField] PuzzleSceneUtility puzzleSceneUtility;
        [SerializeField] public BlaylistManager blaylistManager;
    #endregion

    void Awake()
    {
        FindReferences();

        Subscribe();
    }

    void FindReferences()
    {
        puzzleHandler = PuzzleHandler.GetInstance();
        uiHandler = UIHandler.GetInstance();
        statsHandler = StatsHandler.GetInstance();
        eventHandler = EventHandler.GetInstance();
    }

    void Subscribe()
    {
        eventHandler.onPlace += OnPlace;
        eventHandler.onStartSceneChange += OnStartSceneChange;
    }
    
    void OnPlace()
    {
        moves++;

        CheckBoard();
    }

    void OnStartSceneChange(SceneName sceneName)
    {
        ClearBoard();
    }

    void OnDestroy()
    {
        eventHandler.onPlace -= OnPlace;
        eventHandler.onStartSceneChange -= OnStartSceneChange;
    }

    void Start()
    {   
        CreateBoard(puzzleHandler.GetPuzzleNumber());
    }

    public void CreateBoard(int puzzleID)
    {
        baseString = puzzleHandler.GetPuzzleBase(puzzleID);
        stateString = puzzleHandler.GetPuzzleState(puzzleID);
        solutionString = puzzleHandler.GetPuzzleSolution(puzzleID);

        if(stateString == "") //if new, start from the base
        {
            moves = 0;
            CreateBoard(baseString);
        }
        else if(stateString.Split('|', StringSplitOptions.RemoveEmptyEntries)[0].Equals(solutionString)) //if solved, restart
        {
            stateString = "";
            moves = 0;
            CreateBoard(baseString);
        }
        else //else, an in progress puzzle
        {
            moves = puzzleHandler.GetMoves();
        }

        CreateBoard(stateString == "" ? baseString : stateString); //if no state, create as base, otherwise as the state
    }

    void CreateBoard(string inputString)
    {
        ClearBoard();

        string[] baseBoardAndBlaylist = baseString.Split('|', StringSplitOptions.RemoveEmptyEntries);
        string[] baseBoard = baseBoardAndBlaylist[0].Split(',', StringSplitOptions.RemoveEmptyEntries);
        
        string[] inputBoardAndBlaylist = inputString.Split('|', StringSplitOptions.RemoveEmptyEntries);
        string[] inputBoard = inputBoardAndBlaylist[0].Split(',', StringSplitOptions.RemoveEmptyEntries);

        rows = int.Parse(inputBoard[0]);
        columns = int.Parse(inputBoard[1]);
        tiles = new GameObject[rows, columns];
        
        string[] rootBlays = new string[baseBoard.Length - 2];
        for(int i = 0; i < tiles.Length; i++)
        {
            rootBlays[i] = baseBoard[i + 2];
        }

        string[] blays = new string[inputBoard.Length - 2];
        for(int i = 0; i < tiles.Length; i++)
        {
            blays[i] = inputBoard[i + 2];
        }

        //create physical board
        float columnOffset = ColumnOffset(columns);
        float rowOffset = RowOffset(rows);
        int index = 0;
        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < columns; c++)
            {
                switch(blays[index][0])
                {
                    case('V'):
                    {
                        tiles[r, c] = puzzleSceneUtility.CreateVoidTile(this.gameObject);
                        tiles[r, c].name = "[" + r + ", " + c + "]";
                        break;
                    }
                    case('E'):
                    {
                        tiles[r, c] = puzzleSceneUtility.CreateTile(new Vector3(rowOffset, HEIGHTOFFSET, (columnOffset + (float)c)), this.gameObject);
                        tiles[r, c].name = "[" + r + ", " + c + "]";
                        break;
                    }
                    case('R'):
                    case('O'):
                    case('Y'):
                    case('G'):
                    case('B'):
                    {
                        bool isRootBlay = (rootBlays[index][0] == blays[index][0]); //state blays should not be created as roots

                        tiles[r, c] = puzzleSceneUtility.CreateTile(new Vector3(rowOffset, HEIGHTOFFSET, (columnOffset + (float)c)), this.gameObject);
                        tiles[r, c].name = "[" + r + ", " + c + "]";
                        tiles[r, c].GetComponent<Tile>().AddBlay(puzzleSceneUtility.CreateBlay(blays[index], isRootBlay, tiles[r, c]).GetComponent<Blay>());

                        break;
                    }
                    default:
                    {
                        Debug.LogError("Error: Could not instantiate Tile/Blay");
                        break;
                    }
                } 
                index++;             
            }
            rowOffset += 1.0f; 
        }

        blaylistManager.CreateBlaylist(inputBoardAndBlaylist[1]);
    }

    public void ClearBoard()
    {
        for(int i = 0; i < transform.childCount; i++)
        {
            Destroy(transform.GetChild(i).gameObject);
        }
        blaylistManager.Clear();
        Blay.ResetBid();
        Tile.ResetTid();
    }
    
    public void ResetBoard()
    {
        ClearBoard();
        blaylistManager.Clear();
        CreateBoard(baseString);
    }

    public string Format()
    {
        string returnString = "";
        returnString += rows;
        returnString += ",";
        returnString += columns;
        returnString += ",";

        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < columns; c++)
            {
                if(tiles[r,c] == null) continue;
                if(tiles[r,c].GetComponent<Tile>().state == TileState.VOID)
                {
                    returnString += "V0,";
                }
                else if(tiles[r,c].GetComponent<Tile>().state == TileState.EMPTY)
                {
                    returnString += "E0,";
                }
                else 
                {
                    returnString += ("" + tiles[r,c].GetComponent<Tile>().blay.ToStringShort() + ",");
                }
            }
        }

        string blaylistString;
        if(!((blaylistString = blaylistManager.Format()).Equals(""))) 
        {
            returnString += "|";
            returnString += blaylistString;
        }
        return returnString;
    }

    public bool CheckBoard()
    {
        boardString = Format();

        string checkString = boardString.Split('|', StringSplitOptions.RemoveEmptyEntries)[0];

        if(checkString.Equals(solutionString)) 
        {
            Debug.Log("CONGLATURATION!!! YOU HAVE COMPLETED A GREAT GAME AND PROOVED THE JUSTICE OF OUR CULTURE");
            puzzleHandler.SetMoves(moves);
            eventHandler.InvokeWin();
            return true;
        }
        return false;
    }

    //HELPER METHODS
    float RowOffset(int rows) // 'VERTICAL' OFFSET
    {
        return -((float)rows / 2);
    }

    float ColumnOffset(int columns) // 'HORIZONTAL' OFFSET
    {
        return (1 - ((float)(columns) / 2));
    }

}
