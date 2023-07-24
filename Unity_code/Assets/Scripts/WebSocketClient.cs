using UnityEngine;
//Author Kanishkan M S (ME19B192)
using WebSocketSharp;

public class WebSocketClient : MonoBehaviour
{
    private WebSocket ws;

    private Quaternion targetQuaternion; // Target quaternion value
    private bool hasNewQuaternion = false; // Flag indicating if there is a new quaternion value to update


    private void Start()
    {
        // Create a WebSocket instance and set up event handlers
        ws = new WebSocket("ws://192.168.144.42:81/");
        ws.OnMessage += OnMessageReceived;
        ws.OnOpen += OnWebSocketOpen;
        ws.OnMessage += OnMessageReceived;
        ws.OnError += OnWebSocketError;
        ws.OnClose += OnWebSocketClose;
        ws.Connect();
    }

    private void OnWebSocketOpen(object sender, System.EventArgs e)
    {
        Debug.Log("WebSocket connection opened");
    }

    private void OnWebSocketError(object sender, ErrorEventArgs e)
    {
        Debug.LogError("WebSocket error: " + e.Message);
    }

    private void OnWebSocketClose(object sender, CloseEventArgs e)
    {
        Debug.Log("WebSocket connection closed with code: " + e.Code);
    }

    void Update()
    {
        // Check if there is a new quaternion value to update
        if (hasNewQuaternion)
        {
            // Update the quaternion of the GameObject
            transform.rotation = Quaternion.Inverse(targetQuaternion);

            // Reset the flag
            hasNewQuaternion = false;
        }
    }

    private void OnMessageReceived(object sender, MessageEventArgs e)
    {
        Debug.Log("Received: " + e.Data);
 
        // Process the received data here
        string[] values = e.Data.Split(',');

        if (values.Length >= 4)
        {
            // Parse the quaternion values
            float qw = float.Parse(values[0]);
            float qx = float.Parse(values[1]);
            float qy = float.Parse(values[2]);
            float qz = float.Parse(values[3]);

            // Create the new quaternion
            targetQuaternion = new Quaternion(qx, qz, qy, qw);

            //targetQuaternion = new Quaternion(qx, qy, qz, qw);

            // Set the flag indicating a new quaternion value is available
            hasNewQuaternion = true;
        }
    }

    private void OnDestroy()
    {
        // Close the WebSocket connection when the script is destroyed
        if (ws != null)
        {
            ws.Close();
            ws = null;
        }
    }
}
