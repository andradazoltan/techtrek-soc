-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
-- or if you are using another router e.g. at home, change ID and Password appropriately
SSID = "iPhone"
SSID_PASSWORD = "42069666"

-- I understand from students who have tried, that this also works, using UBC Visitor wireless connection
-- although I haven't tried it

-- SSID= "ubcvisitor"
-- SSID_PASSWORD = ""

-- configure ESP as a station
wifi.setmode(wifi.STATION)
-- wifi.sta.config{ssid=SSID,pwd=SSID_PASSWORD} 3.0.0
wifi.sta.config(SSID,SSID_PASSWORD)
wifi.sta.autoconnect(1)

-- alternatively you could do it this way
-- wifi.sta.config("M112-PD","aiv4aith2Zie4Aeg", 1)
-- wifi.sta.connect()

-- pause for connection to take place - adjust time delay if necessary or repeat until connection made
tmr.delay(1000000) -- wait 1,000,000 us = 1 second

-- This should print 5 if connection was successful
-- print(wifi.sta.status())

-- Prints the IP given to ESP8266
-- print(wifi.sta.getip())

-- List all available wireless network ---
-- See documentation: https://nodemcu.readthedocs.io/en/master/en/modules/wifi/#wifistagetap



function build_post_request(host, uri, data_table)

     data = ""

     for param,value in pairs(data_table) do
          data = data .. param.."="..value.."&"
     end

     request = "POST "..uri.." HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: application/x-www-form-urlencoded\r\n"..
     "Content-Length: "..string.len(data).."\r\n"..
     "\r\n"..
     data
     print(request)
     return request
end


-- This function registers a function to echo back any response from the server, to our DE1/NIOS system 
-- or hyper-terminal (depending on what the dongle is connected to)
function display(sck,response)
     print(response)
end


------ TECHTREK POST GPS TO AWS SERVER ---------------

SERVER_HOST = ec2-18-222-96-240.us-east-2.compute.amazonaws.com
GPS_URI = gps/1

function post_gps(lat, long)

    data = {
     latitude: lat,
     longitude: long
    }

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,GPS_HOST)

    socket:on("connection",function(sck)
         post_request = build_post_request(GPS_HOST,GPS_URI,data)
         sck:send(post_request)
    end)
end


------- TECHTREK GET WEATHER INFORMATION FROM AWS SERVER --------------

WEATHER_URL = weather/firmware/1

function get_weather()

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,HOST)

    -- send get request
    socket:on("connection", function(socket)
        socket:send("GET /"..WEATHER_URL.." HTTP/1.0\r\n"..
              "Host: "..SERVER_HOST.."\r\n"..
              "Connection: close\r\n"..
              "Accept-Charset: utf-8\r\n"..
              "Accept-Encoding: \r\n"..
              "Accept: */*\r\n\r\n")
    end)


end

----------------- SEND SMS -----------------
TWILIO_ACCOUNT_SID = "AC49b57a0c0999b28073363a6c2ed7196a"
TWILIO_TOKEN =       "ee75e72f9e9c4698522449d3d4ed2e36"

HOST = "iot-https-relay.appspot.com" 
URI = "/twilio/Messages.json"

function send_sms(from,to,body)

    data = {
     sid = TWILIO_ACCOUNT_SID,
     token = TWILIO_TOKEN,
     Body = string.gsub(body," ","+"),
     From = from,
     To = to
    }

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,HOST)

    socket:on("connection",function(sck)
         post_request = build_post_request(HOST,URI,data)
         sck:send(post_request)
    end)
end


function check_wifi()
  ip = wifi.sta.getip()

 if(ip==nil) then
   print("Connecting...")
 else
  -- tmr.stop() 3.0.0
  tmr.stop(0)
  print("Connected to AP!")
  print(ip)
  -- send a text message from, to, text
  send_sms("(201) 482-7451","(778) 865-7321","Ring-Ring - this is your breadboard calling!!!!")

 end
end
