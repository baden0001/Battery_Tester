﻿'Programmer:  Will Baden
'Date: 10/26/14
'Description:  This program was written to take in data that is sent from an Arduino tester and feed it into a csv file that can be 
'               Parsed via an Excel program written by the students.  The following protocol shall be used (data sent in ASCII code for student recognition):
'               Control Word, VarA, VarB, VarC, VarD, VarE
'               Control Word = Use this to identify how to parse the following data
'                       Beginning = states the system has started and will display the first timestamp
'                           VarA = Timestamp in milliseconds
'                           VarB - VarE = Not used
'                       Program Run = states the system is running and is sending data
'                           VarA = Timestamp in milliseconds
'                           VarB = Battery Voltage
'                           VarC = Circuit Current
'                           VarD = Relay Status
'                               Will be displayed in simple abcdefg formate where:
'                               a = Relay 1
'                               b = Relay 2
'                               c = Relay 3
'                               d = Relay 4
'                               e = Relay 5
'                               f = Relay 6
'                               g = Relay 7
'                       Program End = states the system has ended and will give the final status update
'                           VarA = Timestamp in milliseconds
'                           VarB = Battery Voltage
'                           VarC = Circuit Current
'                           VarD = Relay Status
'                               Will be displayed in simple abcdefg formate where:
'                               a = Relay 1
'                               b = Relay 2
'                               c = Relay 3
'                               d = Relay 4
'                               e = Relay 5
'                               f = Relay 6
'                               g = Relay 7
'                       System Idle = states the system is idle and waiting to start test.  This will be sent once every 5 seconds
'                           until it is acknowledged by receiving 'Acknowledge' from outside program via the port
'                           After the 'Acknowledge' is sent, sending an 'Start' ASCII character will push the Arduino to start the battery test.
'
'               The Arduino program can take in commands also:
'                   Acknowledge : This is used to acknowledge that the COM port selected is correct
'                   Start : Start the battery test after acknowledgement is complete
'                   Exit : Stop the battery test mid stream
'                   Status : Query the Arduino what the status is.
'                       Response : "Send Start to begin" 
'               If unrecognized commands are seen by the Arduino then the following messages will be sent back:
'                   Arduino waiting for Acknowledgement:
'                       Need to Acknowledge first
'                   Arduino waiting for "Start" command:
'                       Unknown Command
'                   During Battery Testing:
'                       Test Running
'
'Operation Flow:
'   Plug in Arduino to computer
'   Start Program
'   Select the Arduino COM port from the drop down menu
'   Click on "Connect to COM"
'   Program will acknowledge that the port is correct
'       Once program has received acknowledgement, there are multiple commands that could be sent to the Arduino
'       Start
'           Start button will become active and the user can start the test when they are ready
'           Start button is pressed and the Arduino will begin test and continue until the specs of the depleted battery are acheived
'           The Arduino will send out an end of test message and will await to begin testing again
'           Program will allow user to save the data to a CSV file.
'       Ohms
'           Responds back with the resistance values set in memory
'       Offset - Accesses offset setting and reading
'               Response - Set or Read?
'           Set
'               Format - Set,<voltage offset>,<amp offset>
'                   <voltage offset> = numeric value -9999 to 9999
'                   <current offset> = numeric value -9999 to 9999
'               Response Successful- Offsets Accepted
'               Response Fail = Offsets Invalid
'               Allow to set the Voltage Offset (first) and Amp Offset (second).  This will be added/subtracted to the value that
'               is read from the analogs           
'           Read
'               Format - Read
'               Response - Read,<voltage offset>,<amp offset>
'               Reads the voltage offset and amp offset that are preset in EEPROM on Arduino
'
'Revisions:
'   V0.00       Initial write of program
'   V0.01       CSV file saving was added
'               Graphing Capability was added
'   V0.02       Adjusted Scaling on Voltage and Current Graph
'   V0.03       Added Carriage Return as end of message from Arduino
'               TODO add Carriage return to commands sent from PC
'               Added offset reading and writing of values from the Arduino
'                   This will be accessed automatically on startup and dumped into labels on the main screen.
'               Added Variables:
'                   bolOffsetRead, strVoltageOffset, strAmpOffset, strSetVoltageOffset, strSetAmpOffset
'				Added Voltage and Amp offsets that can be set from the main screen.


Public Class MainForm
    'Use to retain data sent from Arduino battery tester
    Dim arrBatteryData(6, 0) As String

    'Use to point to the array location
    Dim intArrBattPointer As Integer = -1

    'Start Time timestamp
    Dim intStartTime As Integer

    'End Time timestamp
    Dim intEndTime As Integer

    'Data Received to be displayed in the text box
    Dim strData As String

    'Control UI via these containers.  This is used to link the serial thread with the UI thread
    Dim bolbtnStart As Boolean = False
    Dim bolbtnSaveCSV As Boolean = False
    Dim bolbtnStop As Boolean = False

    'Use this array as a serial data container that will save data from the serial data received sub and allows the update timer to have access to
    ' this data
    Dim arrSplit(6) As String

    'Time container
    Dim intlastKnownTime As Integer = 0

    'Define what colors to use for amps and volts
    Dim brushAmp As Brush = Brushes.Red
    Dim penAmp As Pen = Pens.Red
    Dim brushVolt As Brush = Brushes.Blue
    Dim penVolt As Pen = Pens.Blue

    'Define if the PC is trying to set or read the voltage and amp offsets
    '   True = read offsets from Arduino (default)
    '   False = set offsets in Arduino
    Dim bolOffsetRead As Boolean = True

    'Use as containers to pass voltage and amp offsets to main form from Serial Thread
    Dim strVoltageOffset As String = "No Data"
    Dim strAmpOffset As String = "No Data"

    'Use as containers to pass voltage and amp offsets to Serial Thread from main form
    Dim strSetVoltageOffset As String
    Dim strSetAmpOffset As String

    'Used to show the informational message once prior to setting the offsets
    Dim bolVoltageOffsetBalloon As Boolean = False
    Dim bolAmpOffsetBalloon As Boolean = False

    Private Sub MainForm_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        UpdateTimer.Start()
        GetSerialPortNames()
        If cmbbxCOMPort.Items.Count <> 0 Then
            cmbbxCOMPort.SelectedIndex = 0
            btnCOM.Enabled = True
            btnStart.Enabled = False
        Else
            MessageBox.Show("Usable COM ports were not detected.")
            btnCOM.Enabled = False
        End If
    End Sub

    Private Sub GetSerialPortNames()
        For Each sp As String In My.Computer.Ports.SerialPortNames
            cmbbxCOMPort.Items.Add(sp)
        Next
    End Sub

    Private Sub btnCOM_Click(sender As Object, e As EventArgs) Handles btnCOM.Click

        If btnCOM.Text = "Connect to COM" Then

            Try
                serialPortMain.PortName = cmbbxCOMPort.SelectedItem.ToString
                serialPortMain.BaudRate = 38400
                serialPortMain.Handshake = IO.Ports.Handshake.None
                serialPortMain.Parity = IO.Ports.Parity.None
                serialPortMain.StopBits = IO.Ports.StopBits.One
                serialPortMain.DataBits = 8
                If serialPortMain.IsOpen Then
                    MsgBox("ComPort open")
                Else
                    serialPortMain.Open()
                End If
                btnCOM.Enabled = True
                btnCOM.Text = "Disconnect"
                cmbbxCOMPort.Enabled = False
                bolbtnStart = False

                'enable offset controls
                btnSetOffsets.Enabled = True
                btnReadOffsets.Enabled = True
                txtbxAmpOffset.Enabled = True
                txtbxVoltageOffset.Enabled = True
                lblCurrentAmpOffset.Enabled = True
                lblCurrentVoltageOffset.Enabled = True
                lblAmpOffset.Enabled = True
                lblVoltageOffset.Enabled = True

                'If the Arduino is in a battery test, then stop the test.
                serialPortMain.Write("Exit" & vbCr)

                bolOffsetRead = True
                'May need to delay for a short time prior to sending this command.
                ' Push controller into offset mode to retreive voltage and amp offsets
                serialPortMain.Write("Offset" & vbCr)

            Catch ex As Exception
                MsgBox(ex.ToString)
                btnStart.Enabled = False
                btnCOM.Text = "Connect to COM"
                cmbbxCOMPort.Enabled = True

                'disable offset controls
                btnSetOffsets.Enabled = False
                btnReadOffsets.Enabled = False
                txtbxAmpOffset.Enabled = False
                txtbxVoltageOffset.Enabled = False
                lblCurrentAmpOffset.Enabled = False
                lblCurrentVoltageOffset.Enabled = False
                lblAmpOffset.Enabled = False
                lblVoltageOffset.Enabled = False
            End Try

        ElseIf btnCOM.Text = "Disconnect" Then
            If serialPortMain.IsOpen Then
                serialPortMain.Close()
            End If
            btnCOM.Text = "Connect to COM"
            bolbtnStart = False
            bolbtnStop = False
            cmbbxCOMPort.Enabled = True

            'disable offset controls
            btnSetOffsets.Enabled = False
            btnReadOffsets.Enabled = False
            txtbxAmpOffset.Enabled = False
            txtbxVoltageOffset.Enabled = False
            lblCurrentAmpOffset.Enabled = False
            lblCurrentVoltageOffset.Enabled = False
            lblAmpOffset.Enabled = False
            lblVoltageOffset.Enabled = False

        End If
    End Sub

    'Sub that will read in data from serial port once it has been received.  Place data into containers that will be used by Update Timer
    Private Sub Serial_Port_Receive(sender As Object, e As EventArgs) Handles serialPortMain.DataReceived

        'read data in from com port and place into usable variable
        Try
            If serialPortMain.IsOpen Then
                Dim dataInput = serialPortMain.ReadLine()
                Dim tmparrSplit() As String = Split(dataInput, ",")
                Dim arrSplitPointer As Integer = 0

                Do While tmparrSplit.Length > arrSplitPointer
                    tmparrSplit(arrSplitPointer) = Replace(tmparrSplit(arrSplitPointer), vbCr, "")
                    arrSplit(arrSplitPointer) = tmparrSplit(arrSplitPointer)
                    arrSplitPointer += 1
                Loop

                'Display data read in from serial port.  Only allow 250 characters to be seen in this textbox
                strData = dataInput & vbCrLf & strData

                If strData.Length > 250 Then
                    strData = strData.Remove(250)
                End If

                'parse the data and display it in the textbox
                '   Open COM Port and request user to start Arduino
                '   Once Arduino shows a system idle status then request user to start test
                '   Once user starts test, then open csv file and start writing data from Arduino to file, line by line.
                '   Display data on the graph.  Also show raw data in text box below
                '   Detect if the end of the program has happened.  If so, then stop logging data, allow user to save data to CSV file.

                'Save the beginning timestamp
                If arrSplit(0) = "Beginning" Then
                    intStartTime = arrSplit(1)
                    arrSplit(1) = 0
                    'clear graph and display start time
                    saveData()
                    clearCurrentData()

                ElseIf arrSplit(0) = "Program Run" Then
                    'Display Data on Screen, resize graph x
                    intlastKnownTime = (arrSplit(1) - intStartTime) / 1000
                    'Save the time (seconds) to the array
                    arrSplit(1) = Math.Round(intlastKnownTime).ToString
                    bolbtnStart = False
                    bolbtnStop = True
                    bolbtnSaveCSV = False
                    saveData()
                    clearCurrentData()

                ElseIf arrSplit(0) = "Program End" Then
                    Dim intTotalTime As Integer = 0
                    intEndTime = arrSplit(1)
                    intTotalTime = intEndTime - intStartTime
                    arrSplit(1) = Math.Round(intTotalTime / 1000).ToString
                    'turn on the save to file function to allow for data to be saved
                    bolbtnSaveCSV = True
                    bolbtnStart = True
                    bolbtnStop = False
                    saveData()
                    clearCurrentData()

                ElseIf arrSplit(0) = "System Idle" Then
                    'send acknowledgement back to Arduino to state that it is ready to start the process
                    serialPortMain.Write("Acknowledge" & vbCr)
                    bolbtnStart = True
                    bolbtnStop = False
                    clearCurrentData()

                ElseIf arrSplit(0) = "Need to Acknowledge first" Then
                    'Send acknowledgement back to Arduino to state that it is ready to start the process
                    serialPortMain.Write("Acknowledge" & vbCr)
                    bolbtnStart = True
                    bolbtnStop = False
                    bolbtnSaveCSV = False
                    clearCurrentData()

                ElseIf arrSplit(0) = "Send Start to begin" Then
                    'Arduino is in state to begin testing
                    bolbtnStart = True
                    bolbtnStop = False
                    'bolbtnSaveCSV = False
                    clearCurrentData()

                ElseIf arrSplit(0) = "Unknown Command" Then
                    'Due to the Arduino already being in the "start to begin" state, kick this program over also
                    bolbtnStart = True
                    bolbtnStop = False
                    bolbtnSaveCSV = False
                    clearCurrentData()

                ElseIf arrSplit(0) = "Set or Read?" Then
                    'bolOffsetRead was set prior to this point.
                    '   Save Offset button sets bolOffsetRead to false
                    '   Connect to Com button sets bolOffsetRead to true
                    If bolOffsetRead Then
                        serialPortMain.Write("Read" & vbCr)
                    ElseIf bolOffsetRead = False Then
                        'Send offsets to Arduino
                        serialPortMain.Write("Set," & strSetVoltageOffset & "," & strSetAmpOffset & vbCr)
                        'serialPortMain.Write(strSetVoltageOffset & " " & strSetAmpOffset & vbCr)

                    End If

                ElseIf arrSplit(0) = "Read" Then
                    'Parse voltage offset and amp offset
                    strVoltageOffset = arrSplit(1)
                    strAmpOffset = arrSplit(2)

                ElseIf arrSplit(0) = "Offsets Accepted" Then
                    'No action required with this response

                ElseIf arrSplit(0) = "Offsets Invalid" Then
                    'Warn user that this failed
                    MessageBox.Show("Offsets were not accepted by tester.")

                Else
                    'Response from Arduino was not understood

                End If
            End If
        Catch ex As Exception
            MessageBox.Show("Serial Data Received closed out")
        End Try

    End Sub

    Private Sub btnSaveCSV_Click(sender As Object, e As EventArgs) Handles btnSaveCSV.Click
        'Dialog box for saving the data to a csv
        Dim saveFileDialog1 As New SaveFileDialog()
        saveFileDialog1.Filter = "CSV Files|*.csv"
        saveFileDialog1.Title = "Save Current Battery Test"

        If saveFileDialog1.ShowDialog() = System.Windows.Forms.DialogResult.OK Then
            If My.Computer.FileSystem.FileExists(saveFileDialog1.FileName) Then
                My.Computer.FileSystem.DeleteFile(saveFileDialog1.FileName)
            End If
            saveToCSV(saveFileDialog1.FileName)
        End If
    End Sub

    Private Sub btnStart_Click(sender As Object, e As EventArgs) Handles btnStart.Click
        'Tell Arduino to start the test
        clearArrBatteryData()
        serialPortMain.Write("Start" & vbCr)
        bolbtnStart = False
        bolbtnStop = True
        bolbtnSaveCSV = False

    End Sub

    Private Sub btnStop_Click(sender As Object, e As EventArgs) Handles btnStop.Click
        'Tell Arduino to stop the test

        serialPortMain.Write("Exit" & vbCr)
        bolbtnStart = True
        bolbtnStop = False

    End Sub

    Private Sub saveData()
        'increment the battery pointer and redim the array size
        intArrBattPointer += 1
        ReDim Preserve arrBatteryData(6, intArrBattPointer)

        'Save data to internal battery data array,
        Dim pointer As Integer = 0
        Do While pointer < 7
            arrBatteryData(pointer, intArrBattPointer) = arrSplit(pointer)
            pointer += 1
        Loop

        'Redraw graph with points
        pnlGraph.Invalidate()
    End Sub

    Private Sub clearCurrentData()
        'Clear out current array data
        ' current array data arrSplit contains the data from the last serially received data
        Dim clearArrayPointer As Integer = 0

        Do While clearArrayPointer <> 6
            arrSplit(clearArrayPointer) = Nothing
            clearArrayPointer += 1
        Loop

    End Sub

    Private Sub clearArrBatteryData()
        'Clear out battery array data and redim the size to beginning state
        ReDim arrBatteryData(6, 0)
        arrBatteryData(0, 0) = "Test Status"
        arrBatteryData(1, 0) = "Time (Seconds)"
        arrBatteryData(2, 0) = "Battery Voltage"
        arrBatteryData(3, 0) = "Battery Current"
        arrBatteryData(4, 0) = "Resistor Bank Status"
        arrBatteryData(5, 0) = "Actual USB Voltage on Arduino"
        intArrBattPointer = 0

    End Sub

    Private Sub saveToCSV(ByVal fileName As String)
        Dim tmp As System.IO.FileStream = System.IO.File.OpenWrite(fileName)
        Dim objWriter As System.IO.StreamWriter = New System.IO.StreamWriter(tmp)
        Dim saveLine As String = ""
        Dim arraySavePointer As Integer = 0
        Dim arraySecondSavePointer As Integer = 0

        'Save all data from array into csv file
        Do While arraySavePointer <= intArrBattPointer
            Do While arraySecondSavePointer < 6
                If arraySecondSavePointer = 0 Then
                    saveLine = arrBatteryData(arraySecondSavePointer, arraySavePointer)
                Else
                    saveLine = saveLine & "," & arrBatteryData(arraySecondSavePointer, arraySavePointer)
                End If
                arraySecondSavePointer += 1
            Loop
            objWriter.Write(saveLine & vbCr)
            saveLine = ""
            arraySecondSavePointer = 0
            arraySavePointer += 1
        Loop

        objWriter.Close()

    End Sub
    'Use this timer to update the textbox and graphics on the screen.  This will be the transfer between the serial data received thread and UI thread
    Private Sub UpdateTimer_Tick(sender As Object, e As EventArgs) Handles UpdateTimer.Tick

        txtbxConsole.Text = strData
        lblCurrentVoltageOffset.Text = "Voltage Offset = " & strVoltageOffset
        lblCurrentAmpOffset.Text = "Amp Offset = " & strAmpOffset

        If bolbtnSaveCSV Then
            btnSaveCSV.Enabled = True
        Else
            btnSaveCSV.Enabled = False
        End If

        If bolbtnStart Then
            btnStart.Enabled = True
            btnSetOffsets.Enabled = True
            btnReadOffsets.Enabled = True
            txtbxAmpOffset.Enabled = True
            txtbxVoltageOffset.Enabled = True
            lblAmpOffset.Enabled = True
            lblVoltageOffset.Enabled = True
            lblCurrentAmpOffset.Enabled = True
            lblCurrentVoltageOffset.Enabled = True
        Else
            btnStart.Enabled = False
            btnSetOffsets.Enabled = False
            btnReadOffsets.Enabled = False
            txtbxAmpOffset.Enabled = False
            txtbxVoltageOffset.Enabled = False
            lblAmpOffset.Enabled = False
            lblVoltageOffset.Enabled = False
            lblCurrentAmpOffset.Enabled = False
            lblCurrentVoltageOffset.Enabled = False
        End If

        If bolbtnStop Then
            btnStop.Enabled = True
        Else
            btnStop.Enabled = False
        End If

    End Sub

    Private Sub pnlGraph_Paint(sender As Object, e As PaintEventArgs) Handles pnlGraph.Paint
        'Use this to redraw the graph as needed

        'Panel size 400,200
        Dim intAmpUpperLimit As Integer = 410 '19 '410
        Dim intAmpLowerLimit As Integer = 380 '10 '380
        Dim intVoltUpperLimit As Integer = 1300
        Dim intVoltLowerLimit As Integer = 1000

        'This will scale how many pixels per tenth of an amp
        Dim intAmpScale As Decimal = 200 / (intAmpUpperLimit - intAmpLowerLimit)

        'This will scale how many pixels per hundredth of a volt
        Dim intVoltScale As Decimal = 200 / (intVoltUpperLimit - intVoltLowerLimit)

        'To be used in drawing lines
        Dim previousAmpX As Integer = 0
        Dim previousAmpY As Integer = 100
        Dim previousVoltX As Integer = 0
        Dim previousVoltY As Integer = 100

        'Loop that will cycle through the array data
        Dim refreshArrayPointer As Integer = 1  'Skip beginning header data

        'Used for scaling the x axis
        Dim decTimeScale As Decimal
        Dim intTimeRecalculation As Integer = 0

        'If the array is larger than 50 data points, then lock the time scale.  This is to help with the amount of
        '   time the screen takes to refresh
        If intlastKnownTime > 50 Then
            decTimeScale = 8

            'change the refresharraypointer to meet the graph time limitations window
            If intArrBattPointer > 9 Then
                refreshArrayPointer = intArrBattPointer - 10
            Else
                refreshArrayPointer = 1
            End If

            'Capture a state where the program was shut down without stopping the battery testing
            If intArrBattPointer > 0 Then
                intTimeRecalculation = arrBatteryData(1, refreshArrayPointer)
            End If
        Else
            'resize the amount of pixels when the amount of time is smaller than 50
            If intlastKnownTime <> 0 Then
                decTimeScale = Math.Round((400 / intlastKnownTime), 5, MidpointRounding.ToEven)
            Else
                decTimeScale = 400
            End If
        End If

        'If the 2d array only has one entry, then automatically place a zero on the left label
        ' otherwise place the starting time on the left label
        If arrBatteryData.Length > 7 Then
            lblTime1.Text = arrBatteryData(1, refreshArrayPointer).ToString
        Else
            lblTime1.Text = "0"
        End If
        lblTime2.Text = intlastKnownTime.ToString

        Do While refreshArrayPointer <= intArrBattPointer

            'Keep within boundaries of the locked graph size
            If CInt(arrBatteryData(3, refreshArrayPointer)) > intAmpLowerLimit And CInt(arrBatteryData(3, refreshArrayPointer)) < intAmpUpperLimit Then
                Dim currentX As Integer = (Math.Round(CInt(arrBatteryData(1, refreshArrayPointer)) - intTimeRecalculation) * decTimeScale)
                Dim currentY As Integer = 200 - Math.Round(CInt(arrBatteryData(3, refreshArrayPointer) - intAmpLowerLimit) * intAmpScale)

                'capture first point to be painted and make it the same as the previous point to clean up left side of chart
                If refreshArrayPointer = 0 Or arrBatteryData(1, refreshArrayPointer).ToString = lblTime1.Text Then
                    previousAmpX = currentX
                    previousAmpY = currentY
                End If

                e.Graphics.FillEllipse(brushAmp, currentX - 2, currentY - 2, 4, 4)
                e.Graphics.DrawLine(penAmp, currentX, currentY, previousAmpX, previousAmpY)

                'Store new previous data
                previousAmpX = currentX
                previousAmpY = currentY
            End If

            'Keep within boundaries of the locked graph size
            If CInt(arrBatteryData(2, refreshArrayPointer)) > intVoltLowerLimit And CInt(arrBatteryData(2, refreshArrayPointer)) < intVoltUpperLimit Then
                Dim currentVoltX As Integer = (Math.Round(CInt(arrBatteryData(1, refreshArrayPointer)) - intTimeRecalculation) * decTimeScale)
                Dim currentVoltY As Integer = 200 - Math.Round(CInt(arrBatteryData(2, refreshArrayPointer) - intVoltLowerLimit) * intVoltScale)

                'capture first point to be painted and make it the same as the previous point to clean up left side of chart
                If refreshArrayPointer = 0 Or arrBatteryData(1, refreshArrayPointer).ToString = lblTime1.Text Then
                    previousVoltX = currentVoltX
                    previousVoltY = currentVoltY
                End If

                e.Graphics.FillEllipse(brushVolt, currentVoltX - 2, currentVoltY - 2, 4, 4)
                e.Graphics.DrawLine(penVolt, currentVoltX, currentVoltY, previousVoltX, previousVoltY)

                'Store new previous data
                previousVoltX = currentVoltX
                previousVoltY = currentVoltY
            End If

            refreshArrayPointer += 1
        Loop

    End Sub

    Private Sub btnSetOffsets_Click(sender As Object, e As EventArgs) Handles btnSetOffsets.Click

        Dim ampValue As Integer
        Dim voltValue As Integer
        If Not Integer.TryParse(txtbxAmpOffset.Text, ampValue) OrElse ampValue < -9999 OrElse ampValue > 9999 _
            OrElse Not Integer.TryParse(txtbxVoltageOffset.Text, voltValue) OrElse voltValue < -9999 OrElse voltValue > 9999 Then
            MessageBox.Show("Please Enter a Number from -9999 to 9999 for Amp Offset and Current Offset")
        Else
            'Push Arduino to setting offset state.  From here the data received sub will see the response from the Arduino.
            If serialPortMain.IsOpen Then
                strSetVoltageOffset = txtbxVoltageOffset.Text
                strSetAmpOffset = txtbxAmpOffset.Text
                serialPortMain.Write("Offset" & vbCr)
                bolOffsetRead = False
            End If
        End If

    End Sub

    Private Sub btnReadOffsets_Click(sender As Object, e As EventArgs) Handles btnReadOffsets.Click
        bolOffsetRead = True
        serialPortMain.Write("Offset" & vbCr)

    End Sub

    Private Sub txtbxVoltageOffset_GotFocus(sender As Object, e As EventArgs) _
     Handles txtbxVoltageOffset.GotFocus

        If Not bolVoltageOffsetBalloon Then
            bolVoltageOffsetBalloon = True
            MessageBox.Show("Input -9999 to 9999 where:" & vbCrLf & "Input = 1234" & vbCrLf & "Represents = 12.34 Volt shift" _
                            & vbCrLf & vbCrLf & "And" & vbCrLf & vbCrLf & "Input = 12" & vbCrLf & "Represents = .12 Volt shift")
        End If

    End Sub

    Private Sub txtbxAmpOffset_GotFocus(sender As Object, e As EventArgs) _
     Handles txtbxAmpOffset.GotFocus

        If Not bolAmpOffsetBalloon Then
            bolAmpOffsetBalloon = True
            MessageBox.Show("Input -9999 to 9999 where:" & vbCrLf & "Input = 1234" & vbCrLf & "Represents = 123.4 Amp shift" _
                            & vbCrLf & vbCrLf & "And" & vbCrLf & vbCrLf & "Input = 12" & vbCrLf & "Represents = 1.2 Amp shift")
        End If

    End Sub

End Class
