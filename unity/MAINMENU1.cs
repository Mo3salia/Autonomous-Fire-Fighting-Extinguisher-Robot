using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
public class MAINMENU1 : MonoBehaviour
{
    
    
    public void start()
    {
       
        SceneManager.LoadScene(2);
    }
    public void QuitGame()
    {

        Application.Quit();
    }
}
