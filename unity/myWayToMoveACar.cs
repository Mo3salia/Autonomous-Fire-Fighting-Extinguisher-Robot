using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using UnityEngine.UI;

public class myWayToMoveACar : MonoBehaviour
{
    [SerializeField] private WheelCollider TR;
    [SerializeField] private WheelCollider TL;
    [SerializeField] private WheelCollider BR;
    [SerializeField] private WheelCollider BL;
    [SerializeField] private Transform TRt;
    [SerializeField] private Transform TLt;
    [SerializeField] private Transform BRt;
    [SerializeField] private Transform BLt;
    [SerializeField] private bool stop=true;
    public string recived_data;
    public int Recived_number = 1;
    public static SerialPort serial = new SerialPort("COM8", 9600);
    public Text changing_text;
    int IsWriting =0;
    
    // Start is called before the first frame update
    void Start()
    {
        serial.Open();
    }
    void readData()
    {
        if (serial.IsOpen)
        {
            recived_data = serial.ReadExisting();
            Recived_number = int.Parse(recived_data);
        }

    }
    // Update is called once per frame
    void FixedUpdate()
    {
        if (Recived_number>10 || Recived_number<1)
        {
            Recived_number=1;
        }
        updatewheels();
        IsWriting = 0;
        while (Recived_number ==1 ) //sraight
        {
            //this.GetComponent<Rigidbody>().velocity = new Vector3(0, 0, 0.5f);
            if(IsWriting == 0)
            {
                changing_text.text = "car is moving straight";
                IsWriting = 1;
            }
            changing_text.text = "car is moving straight";
            TL.motorTorque = 4;
                TR.motorTorque = 4;
                BR.motorTorque = 4;
                BL.motorTorque = 4;
            
            stop = false;
            readData();
        }
        IsWriting = 0;
        while (Recived_number == 2) // right fire
        {
            if(IsWriting == 0)
            {
                changing_text.text = "fire detected at right direction";
                IsWriting=1;
            }
             
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = false;
            transform.Rotate(new Vector3(0, 60, 0) * Time.deltaTime);
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 3) // right d fire
        {
            if (IsWriting == 0)
            {
                changing_text.text = "fire detected at right diagonal direction";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = false;
            transform.Rotate(new Vector3(0, 60, 0) * Time.deltaTime);
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 4) //left fire
        {
            if (IsWriting == 0)
            {
                changing_text.text = "fire detected at left direction";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = false;
            transform.Rotate(new Vector3(0, -60, 0) * Time.deltaTime);
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 5) //left d fire
        {
            if (IsWriting == 0)
            {
                changing_text.text = "fire detected at left diagonal direction";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = false;
            transform.Rotate(new Vector3(0, -60, 0) * Time.deltaTime);
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 6) // right  servo
        {
            if (IsWriting == 0)
            {
                changing_text.text = "LCS detected right direction";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = false;
            transform.Rotate(new Vector3(0, 60, 0) * Time.deltaTime);
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 7) //left servo
        {
            if (IsWriting == 0)
            {
                changing_text.text = "LCS detected left direction";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = false;
            transform.Rotate(new Vector3(0, -60, 0) * Time.deltaTime);
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 8) // fire
        {
            if (IsWriting == 0)
            {
                changing_text.text = "fire detected,Pump Activated ";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = true;
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
           
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 9) // waterlevel +fire
        {
            if (IsWriting == 0)
            {
                changing_text.text = "fire detected,Pump Activated" + "\n\r" +
                "(Water level is low)";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = true;
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            
            readData();

        }
        IsWriting = 0;
        while (Recived_number == 10) // stop for servo reading
        {
            if (IsWriting == 0)
            {
                changing_text.text = "Obstacle Detected .... Scanning all possible ways";
                IsWriting = 1;
            }
            
            this.GetComponent<Rigidbody>().velocity = Vector3.zero;
            stop = true;
            TL.motorTorque = 0;
            TR.motorTorque = 0;
            BR.motorTorque = 0;
            BL.motorTorque = 0;
            readData();


        }
        IsWriting = 0;
    }
    private void updatewheels()
    {
        updateSinglewheel(TL, TLt);
        updateSinglewheel(BR, BRt);
        updateSinglewheel(BL, BLt);
        updateSinglewheel(TR, TRt);
    }
    private void updateSinglewheel(WheelCollider WC, Transform TR)
    {
        Vector3 position;
        Quaternion rot;
        WC.GetWorldPose(out position, out rot);
        if (stop == false)
        {
            TR.Rotate(new Vector3(0, 0, 60) * Time.deltaTime);
        }
        else if (stop == true)
        {
            TR.Rotate(new Vector3(0, 0, 0) * Time.deltaTime);
        }
        
    }
    }
