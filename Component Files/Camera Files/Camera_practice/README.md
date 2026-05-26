## Camera Practice

### LoRa
- Receiver needs to have the `CameraReceive.ino` file uploaded to Ground Station, then run `image_receiver.py` in an interactive console.
- Sender needs to have the `CameraSketch_LoRa.ino` file uploaded to the OBC. Can remain connected for debugging but not necessary.

### Serial [USB]

- Upload `CameraSketch_Serial.ino` to the OBC
- Run `image_receiver.py`. If it fails on invalid start byte, just rerun it. Should receive image every ~30s or so

### Possible improvement
- Have each packet for LoRa send with STARTIMAGE and ENDIMAGE header/footer, to ensure any noise does not affect the transmission
- Set image size via GS command rather than manually preset.