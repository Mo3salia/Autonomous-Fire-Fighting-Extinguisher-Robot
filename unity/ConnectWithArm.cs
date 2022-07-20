using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System.IO.Ports;

public class ConnectWithArm : MonoBehaviour
{
    public static SerialPort serial = new SerialPort("COM8", 9600);
    
    public string recived_data;
    public int Recived_number = 0;
    // Start is called before the first frame update
    void Start()
    {
        
        serial.Open();
    }

    // Update is called once per frame
    void Update()
    {
        if (serial.IsOpen)
        {
           // recive = serial.ReadExisting();
           // name = int.Parse(recive);
        }
    }
}
