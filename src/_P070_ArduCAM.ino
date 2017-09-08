//#######################################################################################################
//#################################### Plugin 070: ArduCAM ##############################################
//#######################################################################################################


#ifdef PLUGIN_BUILD_NORMAL

#include <Wire.h>
#include <SPI.h>
#include <ArduCAM.h>

#define PLUGIN_070
#define PLUGIN_ID_070         70
#define PLUGIN_NAME_070       "ArduCAM OV2640 [TESTING]"
#define PLUGIN_VALUENAME1_070 "Pics"

#define CS_PIN  16
ArduCAM myCAM(OV2640, CS_PIN);

uint32_t number_of_pics = 0;
static const size_t bufferSize = 2048;
static uint8_t buffer[bufferSize] = {0xFF};
uint8_t temp = 0, temp_last = 0;
int i = 0;
bool is_header = false;


boolean Plugin_070(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_070;
        Device[deviceCount].Type = DEVICE_TYPE_I2C;
        Device[deviceCount].VType = SENSOR_TYPE_SINGLE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 1;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = false;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_070);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_070));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {

        // Taskindex[0] - Resolution
        byte choice1 = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
        String options1[9];
        options1[0] = F("160x120");
        options1[1] = F("176x144");
        options1[2] = F("320x240");
        options1[3] = F("352x288");
        options1[4] = F("640x480");
        options1[5] = F("800x600");
        options1[6] = F("1024x768");
        options1[7] = F("1280x1024");
        options1[8] = F("1600x1200");
        int optionValues1[9] = { 0, 1, 2, 3 ,4 ,5 ,6 ,7 ,8 };
        addFormSelector(string, F("Resolution"), F("plugin_070_res"), 9, options1, optionValues1, choice1);

        // Taskindex[1] - Light Mode
        byte choice2 = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        String options2[5];
        options2[0] = F("Auto");
        options2[1] = F("Sunny");
        options2[2] = F("Cloudy");
        options2[3] = F("Office");
        options2[4] = F("Home");
        int optionValues2[5] = { 0, 1, 2, 3 ,4 };
        addFormSelector(string, F("Light mode"), F("plugin_070_lim"), 5, options2, optionValues2, choice2);

        // Taskindex[2] - Brightness
        byte choice3 = Settings.TaskDevicePluginConfig[event->TaskIndex][2];
        String options3[9];
        options3[0] = F("+4");
        options3[1] = F("+3");
        options3[2] = F("+2");
        options3[3] = F("+1");
        options3[4] = F("0");
        options3[5] = F("-1");
        options3[6] = F("-2");
        options3[7] = F("-3");
        options3[8] = F("-4");
        int optionValues3[9] = { 8, 7, 6, 5 ,4 ,3, 2 ,1, 0 };
        addFormSelector(string, F("Brightness"), F("plugin_070_bri"), 9, options3, optionValues3, choice3);

        // Taskindex[3] - Saturation
        byte choice4 = Settings.TaskDevicePluginConfig[event->TaskIndex][3];
        String options4[9];
        options4[0] = F("+4");
        options4[1] = F("+3");
        options4[2] = F("+2");
        options4[3] = F("+1");
        options4[4] = F("0");
        options4[5] = F("-1");
        options4[6] = F("-2");
        options4[7] = F("-3");
        options4[8] = F("-4");
        int optionValues4[9] = { 0, 1, 2, 3 ,4 ,5, 6, 7, 8 };
        addFormSelector(string, F("Saturation"), F("plugin_070_sat"), 9, options4, optionValues4, choice4);

        // Taskindex[4] - Contrast
        byte choice5 = Settings.TaskDevicePluginConfig[event->TaskIndex][4];
        String options5[9];
        options5[0] = F("+4");
        options5[1] = F("+3");
        options5[2] = F("+2");
        options5[3] = F("+1");
        options5[4] = F("0");
        options5[5] = F("-1");
        options5[6] = F("-2");
        options5[7] = F("-3");
        options5[8] = F("-4");
        int optionValues5[9] = { 0, 1, 2, 3 ,4 ,5, 6, 7, 8 };
        addFormSelector(string, F("Contrast"), F("plugin_070_con"), 9, options5, optionValues5, choice5);

        // Taskindex[5] - Special Effects
        byte choice6 = Settings.TaskDevicePluginConfig[event->TaskIndex][5];
        String options6[13];
        options6[0] = F("Antique");
        options6[1] = F("Bluish");
        options6[2] = F("Greenish");
        options6[3] = F("Reddish");
        options6[4] = F("BW");
        options6[5] = F("Negative");
        options6[6] = F("BWnegative");
        options6[7] = F("Normal");
        options6[8] = F("Sepia");
        options6[9] = F("Overexposure");
        options6[10] = F("Solarize");
        options6[11] = F("Blueish");
        options6[12] = F("Yellowish");
        int optionValues6[13] = { 0, 1, 2, 3 ,4 ,5, 6, 7, 8, 9, 10, 11, 12 };
        addFormSelector(string, F("Special effects"), F("plugin_070_eff"), 13, options6, optionValues6, choice6);

        addFormSubHeader(string, F("Picture"));
        addRowLabel(string, F("Test it"));
        addButton(string, F("/control?cmd=arducam,capture"), F(" > Click < "));
        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        Settings.TaskDevicePluginConfig[event->TaskIndex][0] = getFormItemInt(F("plugin_070_res"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][1] = getFormItemInt(F("plugin_070_lim"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][2] = getFormItemInt(F("plugin_070_bri"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][3] = getFormItemInt(F("plugin_070_sat"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][4] = getFormItemInt(F("plugin_070_con"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][5] = getFormItemInt(F("plugin_070_eff"));
        
        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        pinMode(CS_PIN, OUTPUT);
        if (plugin070_check_SPI_bus() && plugin070_check_ov2640())
        {
          myCAM.set_format(JPEG);
          myCAM.InitCAM();
          myCAM.OV2640_set_JPEG_size(Settings.TaskDevicePluginConfig[event->TaskIndex][0]);
          myCAM.OV2640_set_Light_Mode(Settings.TaskDevicePluginConfig[event->TaskIndex][1]);
          myCAM.OV2640_set_Brightness(Settings.TaskDevicePluginConfig[event->TaskIndex][2]);
          myCAM.OV2640_set_Color_Saturation(Settings.TaskDevicePluginConfig[event->TaskIndex][3]);
          myCAM.OV2640_set_Contrast(Settings.TaskDevicePluginConfig[event->TaskIndex][4]);
          myCAM.OV2640_set_Special_effects(Settings.TaskDevicePluginConfig[event->TaskIndex][5]);
          myCAM.clear_fifo_flag();
          String log = F("CAM  : ArduCAM plugin started");
          addLog(LOG_LEVEL_INFO, log);
          success = true;
          break;
        } else {
          break;
        }
        
      }

    case PLUGIN_WRITE:
      {
        String command = parseString(string, 1);
        if (command == F("arducam"))
        {
          String option = parseString(string, 2);
          if (option == F("capture"))
          {
            WiFiClient client = WebServer.client();
            number_of_pics += 1;
            UserVar[event->BaseVarIndex] = number_of_pics;
            success = true;
            String log = F("CAM  : Take a picture");
            addLog(LOG_LEVEL_INFO, log);
            uint32_t len = plugin070_start_capture();
              
            if (len >= MAX_FIFO_SIZE)   // 8M
            {
              log = F("CAM  : Picture is over size!");
              addLog(LOG_LEVEL_ERROR, log);
            }
            else if (len == 0 )             //0 kb
            {
              log = F("CAM  : Picture size is 0!");
              addLog(LOG_LEVEL_ERROR, log);
            }
            else
            {
              log = F("CAM  : Length: ");
              log += len;
              addLog(LOG_LEVEL_INFO, log);
            }
            myCAM.CS_LOW();
            myCAM.set_fifo_burst(); 
            String response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: image/jpeg\r\n";
            response += "Content-len: " + String(len) + "\r\n\r\n";
            WebServer.sendContent(response);
            i = 0;
            while ( len-- )
            {
              temp_last = temp;
              temp =  SPI.transfer(0x00);
              //Read JPEG data from FIFO
              if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
              {
                buffer[i++] = temp;  //save the last  0XD9     
                //Write the remain bytes in the buffer
                //if (!client.connected()) break;
                client.write(&buffer[0], i);
                is_header = false;
                i = 0;
                myCAM.CS_HIGH();
                break; 
              }
              if (is_header == true)
              { 
                //Write image data to buffer if not full
                if (i < bufferSize)
                  buffer[i++] = temp;
                else
                {
                  //Write bufferSize bytes image data to file
                  //if (!client.connected()) break;
                  client.write(&buffer[0], bufferSize);
                  i = 0;
                  buffer[i++] = temp;
                }        
              }
              else if ((temp == 0xD8) & (temp_last == 0xFF))
              {
                is_header = true;
                buffer[i++] = temp_last;
                buffer[i++] = temp;
              }
            } 
          }
          if (option == F("res"))
          {
            success = true;
            plugin070_set_cam_resolution(event->Par1);
          }
          if (option == F("lim"))
          {
            success = true;
            plugin070_set_cam_light_mode(event->Par1);
          }
          if (option == F("bri"))
          {
            success = true;
            plugin070_set_cam_brightness(event->Par1);
          }
          if (option == F("sat"))
          {
            success = true;
            plugin070_set_cam_saturation(event->Par1);
          }
          if (option == F("eff"))
          {
            success = true;
            plugin070_set_cam_special_effects(event->Par1);
          }
        }
      }
  }
  return success;
}

boolean plugin070_check_ov2640()
{
  uint8_t i = 0;
  uint8_t vid, pid;
  boolean ret = false;
  while (i <= 2)
  {
    String log = F("CAM  : camera module OV2640 ");
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
    {
      i += 1;
      log += F("not found! Retry: ");
      log += i;
      delay(1000);
    } else {
      i = 5;
      log += F("found :)");
      ret = true;
    }
    addLog(LOG_LEVEL_INFO, log);
  }
  return ret;
}

boolean plugin070_check_SPI_bus()
{
  uint8_t temp;
  uint8_t i = 0;
  boolean ret = false;
  while(i <= 2)
  {
    String log = F("CAM  : SPI bus ");
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55)
    {
      i += 1;
      log += F("Error! Retry: ");
      log += i;
      delay(1000);
    } else {
      i = 5;
      log += F("okay.");
      ret = true;
    }
    addLog(LOG_LEVEL_INFO, log);
  }
  return ret;
}

void plugin070_set_cam_resolution(uint8_t r)
{
  String log = "";
  if (r >= 0 && r <= 8)
  {
    myCAM.OV2640_set_JPEG_size(r);
    log += F("CAM  : Resolution changed");
  } else {
    log += F("CAM  : Resolution not changed! Invalid option: ");
    log += r;
  }
  addLog(LOG_LEVEL_INFO, log);
}

void plugin070_set_cam_light_mode(uint8_t m)
{
    String log = "";
  if (m >= 0 && m <= 4)
  {
    myCAM.OV2640_set_Light_Mode(m);
    log += F("CAM  : Light mode changed");
  } else {
    log += F("CAM  : Light mode not changed! Invalid option: ");
    log += m;
  }
  addLog(LOG_LEVEL_INFO, log);
}


void plugin070_set_cam_brightness(uint8_t b)
{
  String log = "";
  if (b >= 0 && b <= 8)
  {
    myCAM.OV2640_set_Brightness(b);
    log += F("CAM  : Light mode changed");
  } else {
    log += F("CAM  : Light mode not changed! Invalid option: ");
    log += b;
  }
  addLog(LOG_LEVEL_INFO, log);
}


void plugin070_set_cam_saturation(uint8_t s)
{
  String log = "";
  if (s >= 0 && s <= 8)
  {
    myCAM.OV2640_set_Color_Saturation(s);
    log += F("CAM  : Saturation changed");
  } else {
    log += F("CAM  : Saturation not changed! Invalid option: ");
    log += s;
  }
  addLog(LOG_LEVEL_INFO, log);
}

void plugin070_set_cam_special_effects(uint8_t e)
{
  String log = "";
  if (e >= 0 && e <= 8)
  {
    myCAM.OV2640_set_Special_effects(e);
    log += F("CAM  : Special effects changed");
  } else {
    log += F("CAM  : Special Effects not changed! Invalid option: ");
    log += e;
  }
  addLog(LOG_LEVEL_INFO, log);
}


uint32_t plugin070_start_capture()
{
  //myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.start_capture();

  int total_time = millis();
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  total_time = millis() - total_time;
  String log = F("CAM  : Capture time ");
  log += total_time;
  log += F("ms");
  addLog(LOG_LEVEL_INFO, log);

  uint32_t len  = myCAM.read_fifo_length();
  return len;
}


#endif
