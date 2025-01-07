/*
 * Created by SharpDevelop.
 * User: boss
 * Date: 1.12.2024
 * Time: 11:27
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace Video
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox textCom;
		private System.Windows.Forms.TextBox textBaudRate;
		private System.IO.Ports.SerialPort serialPort1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox textgonderilecekveri;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Button btngonder;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox textData;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox textgelenveriler;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.TextBox textBoxADC;
		private System.Windows.Forms.Button LCDreset;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.button1 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.textCom = new System.Windows.Forms.TextBox();
			this.textBaudRate = new System.Windows.Forms.TextBox();
			this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.textgonderilecekveri = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.btngonder = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.textData = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.textgelenveriler = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.textBoxADC = new System.Windows.Forms.TextBox();
			this.LCDreset = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(287, 52);
			this.button1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(81, 33);
			this.button1.TabIndex = 2;
			this.button1.Text = "Enable";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.Button1Click);
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(287, 91);
			this.button2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(81, 33);
			this.button2.TabIndex = 3;
			this.button2.Text = "Disable";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.Button2Click);
			// 
			// textCom
			// 
			this.textCom.Location = new System.Drawing.Point(116, 63);
			this.textCom.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.textCom.Multiline = true;
			this.textCom.Name = "textCom";
			this.textCom.Size = new System.Drawing.Size(100, 22);
			this.textCom.TabIndex = 4;
			this.textCom.Text = "COM7";
			// 
			// textBaudRate
			// 
			this.textBaudRate.Location = new System.Drawing.Point(116, 91);
			this.textBaudRate.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.textBaudRate.Multiline = true;
			this.textBaudRate.Name = "textBaudRate";
			this.textBaudRate.Size = new System.Drawing.Size(100, 22);
			this.textBaudRate.TabIndex = 5;
			this.textBaudRate.Text = "115200";
			// 
			// serialPort1
			// 
			this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort1DataReceived);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(31, 31);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(152, 23);
			this.label1.TabIndex = 6;
			this.label1.Text = "Port Yapılandırması";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(31, 66);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(68, 23);
			this.label2.TabIndex = 7;
			this.label2.Text = "COM NO:";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(31, 91);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(79, 23);
			this.label3.TabIndex = 8;
			this.label3.Text = "Baundrate:";
			// 
			// textgonderilecekveri
			// 
			this.textgonderilecekveri.Location = new System.Drawing.Point(31, 178);
			this.textgonderilecekveri.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.textgonderilecekveri.Multiline = true;
			this.textgonderilecekveri.Name = "textgonderilecekveri";
			this.textgonderilecekveri.Size = new System.Drawing.Size(180, 40);
			this.textgonderilecekveri.TabIndex = 9;
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(31, 153);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(139, 23);
			this.label4.TabIndex = 10;
			this.label4.Text = "Başlangıç Saati";
			this.label4.Click += new System.EventHandler(this.Label4Click);
			// 
			// btngonder
			// 
			this.btngonder.Location = new System.Drawing.Point(261, 178);
			this.btngonder.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.btngonder.Name = "btngonder";
			this.btngonder.Size = new System.Drawing.Size(107, 39);
			this.btngonder.TabIndex = 11;
			this.btngonder.Text = "Saat Gönder";
			this.btngonder.UseVisualStyleBackColor = true;
			this.btngonder.Click += new System.EventHandler(this.BtngonderClick);
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(31, 119);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(79, 23);
			this.label5.TabIndex = 13;
			this.label5.Text = "DataBlt:";
			// 
			// textData
			// 
			this.textData.Location = new System.Drawing.Point(116, 119);
			this.textData.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.textData.Multiline = true;
			this.textData.Name = "textData";
			this.textData.Size = new System.Drawing.Size(100, 22);
			this.textData.TabIndex = 12;
			this.textData.Text = "8";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(31, 235);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(101, 23);
			this.label6.TabIndex = 14;
			this.label6.Text = "LCD Ekranı";
			// 
			// textgelenveriler
			// 
			this.textgelenveriler.BackColor = System.Drawing.SystemColors.Highlight;
			this.textgelenveriler.ForeColor = System.Drawing.SystemColors.Window;
			this.textgelenveriler.Location = new System.Drawing.Point(31, 261);
			this.textgelenveriler.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.textgelenveriler.Multiline = true;
			this.textgelenveriler.Name = "textgelenveriler";
			this.textgelenveriler.Size = new System.Drawing.Size(180, 40);
			this.textgelenveriler.TabIndex = 15;
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(31, 318);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(101, 23);
			this.label7.TabIndex = 16;
			this.label7.Text = "ADC Verisi";
			this.label7.Click += new System.EventHandler(this.Label7Click);
			// 
			// textBoxADC
			// 
			this.textBoxADC.BackColor = System.Drawing.SystemColors.Highlight;
			this.textBoxADC.ForeColor = System.Drawing.SystemColors.Window;
			this.textBoxADC.Location = new System.Drawing.Point(31, 343);
			this.textBoxADC.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.textBoxADC.Multiline = true;
			this.textBoxADC.Name = "textBoxADC";
			this.textBoxADC.Size = new System.Drawing.Size(180, 40);
			this.textBoxADC.TabIndex = 17;
			// 
			// LCDreset
			// 
			this.LCDreset.Location = new System.Drawing.Point(384, 179);
			this.LCDreset.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.LCDreset.Name = "LCDreset";
			this.LCDreset.Size = new System.Drawing.Size(107, 39);
			this.LCDreset.TabIndex = 18;
			this.LCDreset.Text = "Reset";
			this.LCDreset.UseVisualStyleBackColor = true;
			this.LCDreset.Click += new System.EventHandler(this.LCDresetClick);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(531, 555);
			this.Controls.Add(this.LCDreset);
			this.Controls.Add(this.textBoxADC);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.textgelenveriler);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.textData);
			this.Controls.Add(this.btngonder);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.textgonderilecekveri);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textBaudRate);
			this.Controls.Add(this.textCom);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.Name = "MainForm";
			this.Text = "Video";
			this.ResumeLayout(false);
			this.PerformLayout();

		}
	}
}
