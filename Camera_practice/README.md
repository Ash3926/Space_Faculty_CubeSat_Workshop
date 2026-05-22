## Camera Practice

### LoRa
- Receiver needs to have the `CameraReceive.ino` file uploaded to Arduino, then run `image_receiver.py` in an interactive console.
- Sender needs to have the `CameraSketch_LoRa.ino` file uploaded to the board. Can remain connected for debugging but not necessary.

### Serial [USB]

- Upload `CameraSketch_Serial.ino` to the board
- Run `image_receiver.py`. If it fails on invalid start byte, just rerun it. Should receive image every ~30s or so