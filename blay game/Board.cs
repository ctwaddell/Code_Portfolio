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

        [SerializeField] GameObject tilePrefab;
        [SerializeField] GameObject indicatorPrefab;
        [SerializeField] public BlaylistManager blaylistManager;
        [SerializeField] Material rootMaterial;
        [SerializeField] public GameObject transient;
    #endregion

    void Awake()
    {
        FindReferences();

        eventHandler.onPlace += OnPlace;
    }

    void FindReferences()
    {
        puzzleHandler = PuzzleHandler.GetInstance();
        uiHandler = UIHandler.GetInstance();
        statsHandler = StatsHandler.GetInstance();
        eventHandler = EventHandler.GetInstance();
    }

    void Start()
    {   
        CreateBoard(puzzleHandler.GetPuzzleNumber());
    }

    void OnPlace()
    {
        moves++;

        CheckBoard();
    }

    public void CreateBoard(int puzzleID)
    {
        baseString = puzzleHandler.GetPuzzleBase(puzzleID);
        stateString = puzzleHandler.GetPuzzleState(puzzleID);
        solutionString = puzzleHandler.GetPuzzleSolution(puzzleID);
        //boardString = stateString == "" ? baseString : stateString; //set board to state if it exists else the base string
        if(stateString.Equals(solutionString))
        {
            stateString = "";
            moves = 0;
        } //if won, reset the board
        else
        {
            moves = puzzleHandler.GetMoves();
        }

        CreateBoard(stateString == "" ? baseString : stateString); //if no state, create as base, otherwise as the state
    }

    public void CreateBoard(string inputString)
    {
        ClearBoard();

        string[] baseBoardAndBlays = baseString.Split('|', StringSplitOptions.RemoveEmptyEntries); //
        string[] baseBoardBlays = baseBoardAndBlays[0].Split(',', StringSplitOptions.RemoveEmptyEntries);
        
        string[] boardAndBlays = inputString.Split('|', StringSplitOptions.RemoveEmptyEntries);
        string[] boardBlays = boardAndBlays[0].Split(',', StringSplitOptions.RemoveEmptyEntries);

        rows = int.Parse(boardBlays[0]);
        columns = int.Parse(boardBlays[1]);
        tiles = new GameObject[rows, columns];
        
        string[] baseBlays = new string[baseBoardBlays.Length - 2];
        for(int i = 0; i < tiles.Length; i++)
        {
            baseBlays[i] = baseBoardBlays[i + 2];
        }

        string[] blays = new string[boardBlays.Length - 2];
        for(int i = 0; i < tiles.Length; i++)
        {
            blays[i] = boardBlays[i + 2];
        }


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
                        tiles[r, c] = puzzleSceneUtility.CreateTile(TileState.VOID, this.gameObject);
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
                        bool isRootBlay = (baseBlays[index][0] == blays[index][0]); //state blays should not be created as roots

                        tiles[r, c] = puzzleSceneUtility.CreateTile(new Vector3(rowOffset, HEIGHTOFFSET, (columnOffset + (float)c)), this.gameObject);
                        tiles[r, c].name = "[" + r + ", " + c + "]";
                        tiles[r, c].GetComponent<Tile>().AddBlay(puzzleSceneUtility.CreateBlay(blays[index], isRootBlay, tiles[r, c]).GetComponent<Blay>());

                        break;
                    }
                    default:
                        Debug.Log("Error: Coult not instantiate tile/blay");
                        break;
                } 
                index++;             
            }
            rowOffset += 1.0f; 
        }

        try
        {
            string[] baseBlaylist = baseBoardAndBlays[1].Split(',', StringSplitOptions.RemoveEmptyEntries);
            int baseBlaylistSize = baseBlaylist.Length;
            string[] blaylistBlays = boardAndBlays[1].Split(',', StringSplitOptions.RemoveEmptyEntries);
            int blaylistSize = blaylistBlays.Length;

            List<string> blaylistParam = new List<string>();
            for(int i = 0; i < blaylistSize; i++)
            {
                blaylistParam.Add(blaylistBlays[i]);
            }

            for(int i = 0; i < baseBlaylistSize - blaylistSize; i++)
            {
                blaylistParam.Add("E0");
            }

            blaylistManager.CreateBlaylist(blaylistParam.ToArray());
        }
        catch(Exception e) //if the board is solved, this will cause an error so we catch it
        {

        }
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
