<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class MainForm
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.txtbxConsole = New System.Windows.Forms.TextBox()
        Me.btnStart = New System.Windows.Forms.Button()
        Me.btnStop = New System.Windows.Forms.Button()
        Me.pnlGraph = New System.Windows.Forms.Panel()
        Me.serialPortMain = New System.IO.Ports.SerialPort(Me.components)
        Me.btnCOM = New System.Windows.Forms.Button()
        Me.cmbbxCOMPort = New System.Windows.Forms.ComboBox()
        Me.btnSaveCSV = New System.Windows.Forms.Button()
        Me.UpdateTimer = New System.Windows.Forms.Timer(Me.components)
        Me.lblVoltage3 = New System.Windows.Forms.Label()
        Me.lblVoltage1 = New System.Windows.Forms.Label()
        Me.lblVoltage2 = New System.Windows.Forms.Label()
        Me.lblAmp2 = New System.Windows.Forms.Label()
        Me.lblAmp1 = New System.Windows.Forms.Label()
        Me.lblAmp3 = New System.Windows.Forms.Label()
        Me.lblTime2 = New System.Windows.Forms.Label()
        Me.lblTime1 = New System.Windows.Forms.Label()
        Me.lblVoltColor = New System.Windows.Forms.Label()
        Me.lblAmpColor = New System.Windows.Forms.Label()
        Me.btnSetOffsets = New System.Windows.Forms.Button()
        Me.txtbxVoltageOffset = New System.Windows.Forms.TextBox()
        Me.txtbxAmpOffset = New System.Windows.Forms.TextBox()
        Me.lblVoltageOffset = New System.Windows.Forms.Label()
        Me.lblAmpOffset = New System.Windows.Forms.Label()
        Me.lblCurrentAmpOffset = New System.Windows.Forms.Label()
        Me.lblCurrentVoltageOffset = New System.Windows.Forms.Label()
        Me.btnReadOffsets = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'txtbxConsole
        '
        Me.txtbxConsole.Location = New System.Drawing.Point(12, 259)
        Me.txtbxConsole.Multiline = True
        Me.txtbxConsole.Name = "txtbxConsole"
        Me.txtbxConsole.ReadOnly = True
        Me.txtbxConsole.Size = New System.Drawing.Size(458, 70)
        Me.txtbxConsole.TabIndex = 0
        '
        'btnStart
        '
        Me.btnStart.Enabled = False
        Me.btnStart.Location = New System.Drawing.Point(602, 12)
        Me.btnStart.Name = "btnStart"
        Me.btnStart.Size = New System.Drawing.Size(70, 23)
        Me.btnStart.TabIndex = 1
        Me.btnStart.Text = "Start"
        Me.btnStart.UseVisualStyleBackColor = True
        '
        'btnStop
        '
        Me.btnStop.Enabled = False
        Me.btnStop.Location = New System.Drawing.Point(602, 41)
        Me.btnStop.Name = "btnStop"
        Me.btnStop.Size = New System.Drawing.Size(70, 22)
        Me.btnStop.TabIndex = 2
        Me.btnStop.Text = "Stop"
        Me.btnStop.UseVisualStyleBackColor = True
        '
        'pnlGraph
        '
        Me.pnlGraph.BackColor = System.Drawing.SystemColors.ControlLightLight
        Me.pnlGraph.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.pnlGraph.Location = New System.Drawing.Point(47, 18)
        Me.pnlGraph.Name = "pnlGraph"
        Me.pnlGraph.Size = New System.Drawing.Size(400, 200)
        Me.pnlGraph.TabIndex = 3
        '
        'serialPortMain
        '
        '
        'btnCOM
        '
        Me.btnCOM.Location = New System.Drawing.Point(567, 280)
        Me.btnCOM.Name = "btnCOM"
        Me.btnCOM.Size = New System.Drawing.Size(104, 24)
        Me.btnCOM.TabIndex = 4
        Me.btnCOM.Text = "Connect to COM"
        Me.btnCOM.UseVisualStyleBackColor = True
        '
        'cmbbxCOMPort
        '
        Me.cmbbxCOMPort.FormattingEnabled = True
        Me.cmbbxCOMPort.Location = New System.Drawing.Point(568, 310)
        Me.cmbbxCOMPort.Name = "cmbbxCOMPort"
        Me.cmbbxCOMPort.Size = New System.Drawing.Size(103, 21)
        Me.cmbbxCOMPort.TabIndex = 5
        '
        'btnSaveCSV
        '
        Me.btnSaveCSV.Enabled = False
        Me.btnSaveCSV.Location = New System.Drawing.Point(567, 250)
        Me.btnSaveCSV.Name = "btnSaveCSV"
        Me.btnSaveCSV.Size = New System.Drawing.Size(104, 23)
        Me.btnSaveCSV.TabIndex = 7
        Me.btnSaveCSV.Text = "Save to CSV"
        Me.btnSaveCSV.UseVisualStyleBackColor = True
        '
        'UpdateTimer
        '
        '
        'lblVoltage3
        '
        Me.lblVoltage3.AutoSize = True
        Me.lblVoltage3.Location = New System.Drawing.Point(15, 202)
        Me.lblVoltage3.Name = "lblVoltage3"
        Me.lblVoltage3.Size = New System.Drawing.Size(26, 13)
        Me.lblVoltage3.TabIndex = 8
        Me.lblVoltage3.Text = "10V"
        '
        'lblVoltage1
        '
        Me.lblVoltage1.AutoSize = True
        Me.lblVoltage1.Location = New System.Drawing.Point(15, 9)
        Me.lblVoltage1.Name = "lblVoltage1"
        Me.lblVoltage1.Size = New System.Drawing.Size(26, 13)
        Me.lblVoltage1.TabIndex = 9
        Me.lblVoltage1.Text = "13V"
        '
        'lblVoltage2
        '
        Me.lblVoltage2.AutoSize = True
        Me.lblVoltage2.Location = New System.Drawing.Point(6, 104)
        Me.lblVoltage2.Name = "lblVoltage2"
        Me.lblVoltage2.Size = New System.Drawing.Size(35, 13)
        Me.lblVoltage2.TabIndex = 10
        Me.lblVoltage2.Text = "11.5V"
        '
        'lblAmp2
        '
        Me.lblAmp2.AutoSize = True
        Me.lblAmp2.Location = New System.Drawing.Point(453, 104)
        Me.lblAmp2.Name = "lblAmp2"
        Me.lblAmp2.Size = New System.Drawing.Size(57, 13)
        Me.lblAmp2.TabIndex = 13
        Me.lblAmp2.Text = "38.5 Amps"
        '
        'lblAmp1
        '
        Me.lblAmp1.AutoSize = True
        Me.lblAmp1.Location = New System.Drawing.Point(462, 9)
        Me.lblAmp1.Name = "lblAmp1"
        Me.lblAmp1.Size = New System.Drawing.Size(48, 13)
        Me.lblAmp1.TabIndex = 12
        Me.lblAmp1.Text = "41 Amps"
        '
        'lblAmp3
        '
        Me.lblAmp3.AutoSize = True
        Me.lblAmp3.Location = New System.Drawing.Point(462, 202)
        Me.lblAmp3.Name = "lblAmp3"
        Me.lblAmp3.Size = New System.Drawing.Size(48, 13)
        Me.lblAmp3.TabIndex = 11
        Me.lblAmp3.Text = "38 Amps"
        '
        'lblTime2
        '
        Me.lblTime2.AutoSize = True
        Me.lblTime2.Location = New System.Drawing.Point(418, 226)
        Me.lblTime2.Name = "lblTime2"
        Me.lblTime2.Size = New System.Drawing.Size(13, 13)
        Me.lblTime2.TabIndex = 14
        Me.lblTime2.Text = "0"
        '
        'lblTime1
        '
        Me.lblTime1.AutoSize = True
        Me.lblTime1.Location = New System.Drawing.Point(44, 226)
        Me.lblTime1.Name = "lblTime1"
        Me.lblTime1.Size = New System.Drawing.Size(13, 13)
        Me.lblTime1.TabIndex = 15
        Me.lblTime1.Text = "0"
        '
        'lblVoltColor
        '
        Me.lblVoltColor.AutoSize = True
        Me.lblVoltColor.BackColor = System.Drawing.Color.Blue
        Me.lblVoltColor.ForeColor = System.Drawing.Color.White
        Me.lblVoltColor.Location = New System.Drawing.Point(15, 226)
        Me.lblVoltColor.Name = "lblVoltColor"
        Me.lblVoltColor.Size = New System.Drawing.Size(28, 13)
        Me.lblVoltColor.TabIndex = 16
        Me.lblVoltColor.Text = "Blue"
        '
        'lblAmpColor
        '
        Me.lblAmpColor.AutoSize = True
        Me.lblAmpColor.BackColor = System.Drawing.Color.Red
        Me.lblAmpColor.ForeColor = System.Drawing.Color.White
        Me.lblAmpColor.Location = New System.Drawing.Point(482, 226)
        Me.lblAmpColor.Name = "lblAmpColor"
        Me.lblAmpColor.Size = New System.Drawing.Size(27, 13)
        Me.lblAmpColor.TabIndex = 17
        Me.lblAmpColor.Text = "Red"
        '
        'btnSetOffsets
        '
        Me.btnSetOffsets.Enabled = False
        Me.btnSetOffsets.Location = New System.Drawing.Point(567, 109)
        Me.btnSetOffsets.Name = "btnSetOffsets"
        Me.btnSetOffsets.Size = New System.Drawing.Size(104, 23)
        Me.btnSetOffsets.TabIndex = 18
        Me.btnSetOffsets.Text = "Set Offsets"
        Me.btnSetOffsets.UseVisualStyleBackColor = True
        '
        'txtbxVoltageOffset
        '
        Me.txtbxVoltageOffset.Enabled = False
        Me.txtbxVoltageOffset.Location = New System.Drawing.Point(622, 138)
        Me.txtbxVoltageOffset.Name = "txtbxVoltageOffset"
        Me.txtbxVoltageOffset.Size = New System.Drawing.Size(48, 20)
        Me.txtbxVoltageOffset.TabIndex = 19
        '
        'txtbxAmpOffset
        '
        Me.txtbxAmpOffset.Enabled = False
        Me.txtbxAmpOffset.Location = New System.Drawing.Point(622, 164)
        Me.txtbxAmpOffset.Name = "txtbxAmpOffset"
        Me.txtbxAmpOffset.Size = New System.Drawing.Size(48, 20)
        Me.txtbxAmpOffset.TabIndex = 20
        '
        'lblVoltageOffset
        '
        Me.lblVoltageOffset.AutoSize = True
        Me.lblVoltageOffset.Enabled = False
        Me.lblVoltageOffset.Location = New System.Drawing.Point(539, 141)
        Me.lblVoltageOffset.Name = "lblVoltageOffset"
        Me.lblVoltageOffset.Size = New System.Drawing.Size(74, 13)
        Me.lblVoltageOffset.TabIndex = 21
        Me.lblVoltageOffset.Text = "Voltage Offset"
        '
        'lblAmpOffset
        '
        Me.lblAmpOffset.AutoSize = True
        Me.lblAmpOffset.Enabled = False
        Me.lblAmpOffset.Location = New System.Drawing.Point(554, 167)
        Me.lblAmpOffset.Name = "lblAmpOffset"
        Me.lblAmpOffset.Size = New System.Drawing.Size(59, 13)
        Me.lblAmpOffset.TabIndex = 22
        Me.lblAmpOffset.Text = "Amp Offset"
        '
        'lblCurrentAmpOffset
        '
        Me.lblCurrentAmpOffset.AutoSize = True
        Me.lblCurrentAmpOffset.Location = New System.Drawing.Point(554, 217)
        Me.lblCurrentAmpOffset.Name = "lblCurrentAmpOffset"
        Me.lblCurrentAmpOffset.Size = New System.Drawing.Size(59, 13)
        Me.lblCurrentAmpOffset.TabIndex = 24
        Me.lblCurrentAmpOffset.Text = "Amp Offset"
        '
        'lblCurrentVoltageOffset
        '
        Me.lblCurrentVoltageOffset.AutoSize = True
        Me.lblCurrentVoltageOffset.Location = New System.Drawing.Point(539, 201)
        Me.lblCurrentVoltageOffset.Name = "lblCurrentVoltageOffset"
        Me.lblCurrentVoltageOffset.Size = New System.Drawing.Size(74, 13)
        Me.lblCurrentVoltageOffset.TabIndex = 23
        Me.lblCurrentVoltageOffset.Text = "Voltage Offset"
        '
        'btnReadOffsets
        '
        Me.btnReadOffsets.Enabled = False
        Me.btnReadOffsets.Location = New System.Drawing.Point(567, 81)
        Me.btnReadOffsets.Name = "btnReadOffsets"
        Me.btnReadOffsets.Size = New System.Drawing.Size(104, 23)
        Me.btnReadOffsets.TabIndex = 25
        Me.btnReadOffsets.Text = "Read Offsets"
        Me.btnReadOffsets.UseVisualStyleBackColor = True
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(683, 340)
        Me.Controls.Add(Me.btnReadOffsets)
        Me.Controls.Add(Me.lblCurrentAmpOffset)
        Me.Controls.Add(Me.lblCurrentVoltageOffset)
        Me.Controls.Add(Me.lblAmpOffset)
        Me.Controls.Add(Me.lblVoltageOffset)
        Me.Controls.Add(Me.txtbxAmpOffset)
        Me.Controls.Add(Me.txtbxVoltageOffset)
        Me.Controls.Add(Me.btnSetOffsets)
        Me.Controls.Add(Me.lblAmpColor)
        Me.Controls.Add(Me.lblVoltColor)
        Me.Controls.Add(Me.lblTime1)
        Me.Controls.Add(Me.lblTime2)
        Me.Controls.Add(Me.lblAmp2)
        Me.Controls.Add(Me.lblAmp1)
        Me.Controls.Add(Me.lblAmp3)
        Me.Controls.Add(Me.lblVoltage2)
        Me.Controls.Add(Me.lblVoltage1)
        Me.Controls.Add(Me.lblVoltage3)
        Me.Controls.Add(Me.btnSaveCSV)
        Me.Controls.Add(Me.cmbbxCOMPort)
        Me.Controls.Add(Me.btnCOM)
        Me.Controls.Add(Me.pnlGraph)
        Me.Controls.Add(Me.btnStop)
        Me.Controls.Add(Me.btnStart)
        Me.Controls.Add(Me.txtbxConsole)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D
        Me.Name = "MainForm"
        Me.Text = "Battery Tester"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents txtbxConsole As System.Windows.Forms.TextBox
    Friend WithEvents btnStart As System.Windows.Forms.Button
    Friend WithEvents btnStop As System.Windows.Forms.Button
    Friend WithEvents pnlGraph As System.Windows.Forms.Panel
    Friend WithEvents serialPortMain As System.IO.Ports.SerialPort
    Friend WithEvents btnCOM As System.Windows.Forms.Button
    Friend WithEvents cmbbxCOMPort As System.Windows.Forms.ComboBox
    Friend WithEvents btnSaveCSV As System.Windows.Forms.Button
    Friend WithEvents UpdateTimer As System.Windows.Forms.Timer
    Friend WithEvents lblVoltage3 As System.Windows.Forms.Label
    Friend WithEvents lblVoltage1 As System.Windows.Forms.Label
    Friend WithEvents lblVoltage2 As System.Windows.Forms.Label
    Friend WithEvents lblAmp2 As System.Windows.Forms.Label
    Friend WithEvents lblAmp1 As System.Windows.Forms.Label
    Friend WithEvents lblAmp3 As System.Windows.Forms.Label
    Friend WithEvents lblTime2 As System.Windows.Forms.Label
    Friend WithEvents lblTime1 As System.Windows.Forms.Label
    Friend WithEvents lblVoltColor As System.Windows.Forms.Label
    Friend WithEvents lblAmpColor As System.Windows.Forms.Label
    Friend WithEvents btnSetOffsets As System.Windows.Forms.Button
    Friend WithEvents txtbxVoltageOffset As System.Windows.Forms.TextBox
    Friend WithEvents txtbxAmpOffset As System.Windows.Forms.TextBox
    Friend WithEvents lblVoltageOffset As System.Windows.Forms.Label
    Friend WithEvents lblAmpOffset As System.Windows.Forms.Label
    Friend WithEvents lblCurrentAmpOffset As System.Windows.Forms.Label
    Friend WithEvents lblCurrentVoltageOffset As System.Windows.Forms.Label
    Friend WithEvents btnReadOffsets As System.Windows.Forms.Button

End Class
