/*
 * Created by SharpDevelop.
 * User: boss
 * Date: 1.12.2024
 * Time: 11:27
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Video
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	public partial class MainForm : Form
	{
		string gelenveri=string.Empty;
		
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
		}
		 private void Button1Click(object sender, EventArgs e)
		{
	if (!serialPort1.IsOpen)
            {
                try
                {
                    serialPort1.PortName = textCom.Text; // Kullanıcının seçtiği COM port
                    serialPort1.BaudRate = Convert.ToInt32(textBaudRate.Text); // Baud hızı
                    serialPort1.DataBits = Convert.ToInt32(textData.Text); // Veri bit uzunluğu
                    serialPort1.Open();
                    MessageBox.Show("Bağlantı Başarılı!");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Bağlantı Hatası: " + ex.Message);
                }
            }
		}
		private void Button2Click(object sender, EventArgs e)
		{
			if (serialPort1.IsOpen)
            {
                serialPort1.Close();
                MessageBox.Show("Bağlantı Kesildi!");
            }
		}
		void Label4Click(object sender, EventArgs e)
		{
	
		}
		
		void Label7Click(object sender, EventArgs e)
		{
	
		}
		void BtngonderClick(object sender, EventArgs e)
		{
			string saat = textgonderilecekveri.Text; // Kullanıcının girdiği saat
            if (!string.IsNullOrEmpty(saat) && saat.Length == 8) // Saat formatını kontrol et (hh:mm:ss)
            {
                try
                {
                	
                    serialPort1.WriteLine(saat); // Mikroişlemciye saat bilgisini gönder
                    MessageBox.Show("Saat Bilgisi Gönderildi!");
                    
                    
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Veri Gönderim Hatası: " + ex.Message);
                }
            }
            else
            {
                MessageBox.Show("Lütfen 'hh:mm:ss' formatında geçerli bir saat giriniz!");
            }
		}
		
		
		private void SerialPort1DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
		{
			 try
            {
                gelenveri = serialPort1.ReadLine(); // Mikroişlemciden gelen veriyi oku
                this.Invoke(new EventHandler(DisplayText)); // Gelen veriyi UI'ya gönder
            }
            catch (Exception ex)
            {
                MessageBox.Show("Veri Alma Hatası: " + ex.Message);
            }
		}
		
  private void DisplayText(object sender, EventArgs e)
        {
            // Mikroişlemciden gelen veriyi ayrıştır
            if (gelenveri.StartsWith("Time:"))
            {
                string[] parts = gelenveri.Split(' '); // Veriyi parçala
                if (parts.Length >= 2)
                {
                    // Ekranda saat ve ADC bilgisini güncelle
                    textgelenveriler.Text=  parts[0].Substring(5);
                    textBoxADC.Text =  parts[1].Substring(4);
                }
            }
        }
		void LCDresetClick(object sender, EventArgs e)
		{			try
    {
        if (serialPort1.IsOpen)
        {
            serialPort1.WriteLine("RESET_SYSTEM"); // Tiva'ya reset komutu gönder
            MessageBox.Show("Sistem Resetleme Komutu Gönderildi!");
        }
        else
        {
            MessageBox.Show("Port açık değil! Lütfen önce portu açın.");
        }
    }
    catch (Exception ex)
    {
        MessageBox.Show("Resetleme Hatası: " + ex.Message);
    }
		}
    }
}
