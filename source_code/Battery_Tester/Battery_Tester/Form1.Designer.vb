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
        Me.btnStart.Location = New System.Drawing.Point(570, 30)
        Me.btnStart.Name = "btnStart"
        Me.btnStart.Size = New System.Drawing.Size(70, 23)
        Me.btnStart.TabIndex = 1
        Me.btnStart.Text = "Start"
        Me.btnStart.UseVisualStyleBackColor = True
        '
        'btnStop
        '
        Me.btnStop.Enabled = False
        Me.btnStop.Location = New System.Drawing.Point(570, 59)
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
        Me.btnCOM.Location = New System.Drawing.Point(535, 301)
        Me.btnCOM.Name = "btnCOM"
        Me.btnCOM.Size = New System.Drawing.Size(106, 24)
        Me.btnCOM.TabIndex = 4
        Me.btnCOM.Text = "Connect to COM"
        Me.btnCOM.UseVisualStyleBackColor = True
        '
        'cmbbxCOMPort
        '
        Me.cmbbxCOMPort.FormattingEnabled = True
        Me.cmbbxCOMPort.Location = New System.Drawing.Point(537, 263)
        Me.cmbbxCOMPort.Name = "cmbbxCOMPort"
        Me.cmbbxCOMPort.Size = New System.Drawing.Size(103, 21)
        Me.cmbbxCOMPort.TabIndex = 5
        '
        'btnSaveCSV
        '
        Me.btnSaveCSV.Enabled = False
        Me.btnSaveCSV.Location = New System.Drawing.Point(535, 221)
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
        Me.lblVoltColor.Location = New System.Drawing.Point(15, 226)
        Me.lblVoltColor.Name = "lblVoltColor"
        Me.lblVoltColor.Size = New System.Drawing.Size(28, 13)
        Me.lblVoltColor.TabIndex = 16
        Me.lblVoltColor.Text = "Blue"
        '
        'lblAmpColor
        '
        Me.lblAmpColor.AutoSize = True
        Me.lblAmpColor.Location = New System.Drawing.Point(482, 226)
        Me.lblAmpColor.Name = "lblAmpColor"
        Me.lblAmpColor.Size = New System.Drawing.Size(27, 13)
        Me.lblAmpColor.TabIndex = 17
        Me.lblAmpColor.Text = "Red"
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(653, 340)
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

End Class
