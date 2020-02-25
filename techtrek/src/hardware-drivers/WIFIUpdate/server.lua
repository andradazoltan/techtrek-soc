-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
-- or if you are using another router e.g. at home, change ID and Password appropriately

SSID = "Ash's iPhone"
SSID_PASSWORD = "01234567898"

--  SSID = "TAN-S"
--  SSID_PASSWORD = "6045940350"


-- SSID= "ubcvisitor"
-- SSID_PASSWORD = ""

get_weather_file = "weather.txt"

-- configure ESP as a station
wifi.setmode(wifi.STATION)
-- wifi.sta.config{ssid=SSID,pwd=SSID_PASSWORD} 3.0.0
wifi.sta.config(SSID,SSID_PASSWORD)
wifi.sta.autoconnect(1)

-- alternatively you could do it this way
-- wifi.sta.config("M112-PD","aiv4aith2Zie4Aeg", 1)
-- wifi.sta.connect()

-- pause for connection to take place - adjust time delay if necessary or repeat until connection made
tmr.delay(3000000) -- wait 1,000,000 us = 1 second

-- This should print 5 if connection was successful
--print(wifi.sta.status())

------- Connection Phase -------------
if (wifi.sta.status() == 5) then 
     print("Connected Successfully!")
else 
     print("Could not connect")
end

-- Prints the IP given to ESP8266
-- print(wifi.sta.getip())

-- List all available wireless network ---
-- See documentation: https://nodemcu.readthedocs.io/en/master/en/modules/wifi/#wifistagetap



function build_GPS_request(host, uri, data_table, lat, long)

     data = ""

     request = "POST "..uri.."?latitude=" ..lat.. "&" .."longitude="..long.. " HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: application/x-www-form-urlencoded\r\n"..
     "Content-Length: 0\r\n"..
     "\r\n"..
     data
     print(request)
     return request
end

-- Build a generic GET request
function build_get_request(host, url)

     request = "GET /"..url.." HTTP/1.0\r\n"..
          "Host: "..host.."\r\n"..
          "Connection: close\r\n"..
          "Accept-Charset: utf-8\r\n"..
          "Accept-Encoding: \r\n"..
          "User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n".. 
          "Accept: */*\r\n\r\n"

     return request
end

function build_rating_request(host, uri, data_table, rate)

     data = ""

     request = "POST "..uri.."?score=" ..rate.. " HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: application/x-www-form-urlencoded\r\n"..
     "Content-Length: 0\r\n"..
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

SERVER_HOST = "ec2-18-222-96-240.us-east-2.compute.amazonaws.com"
GPS_URI = "/gps/firmware/1"

-- post_gps(50116322,-122957359)
function post_gps(lat, long)

    --Convert coord back to floats
    lat = lat/1000000
    long = long/1000000
    
    data = ""

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,SERVER_HOST)

    socket:on("connection",function(sck)
         post_request = build_GPS_request(SERVER_HOST,GPS_URI,data, lat, long)
         sck:send(post_request)
    end)
end


------- TECHTREK GET WEATHER INFORMATION FROM AWS SERVER --------------

WEATHER_URL = "weather/firmware/1"

function get_weather()

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,SERVER_HOST)

    -- send get request
    socket:on("connection", function(socket)
          new_request = build_get_request(SERVER_HOST, WEATHER_URL)
          socket:send(new_request)
    end)

end

------- TECHTREK POST HELP --------------

HELP_URI = "/help"

function post_help()
     
     data = ""

     socket = net.createConnection(net.TCP,0)
     socket:on("receive",display)
     socket:connect(80,SERVER_HOST)

     request = "POST "..HELP_URI.." HTTP/1.1\r\n"..
          "Host: "..SERVER_HOST.."\r\n"..
          "Connection: close\r\n"..
          "Content-Type: application/x-www-form-urlencoded\r\n"..
          "Content-Length: "..string.len(data).."\r\n"..
          "\r\n"..
          data
     print(request)
     
     socket:on("connection",function(sck)
          sck:send(request)
     end)

end

------- TECHTREK GET POPULATION FROM AWS SERVER --------------

--
POPULATION_URL = "population/firmware/1"

function get_population()

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,SERVER_HOST)

    -- send get request
    socket:on("connection", function(socket)
          new_request = build_get_request(SERVER_HOST, POPULATION_URL)
          socket:send(new_request)
    end)

end

------- TECHTREK GET WARNINGS FROM AWS SERVER --------------

--Get the lastest warning
WARNINGS_URL = "warning/firmware/1"

function get_warnings()

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,SERVER_HOST)

    -- send get request
    socket:on("connection", function(socket)
          new_request = build_get_request(SERVER_HOST, WARNINGS_URL)
          socket:send(new_request)
    end)

end

------- TECHTREK POST WARNINGS TO AWS SERVER --------------

WARNING_URI = "/warning/1"

-- Example: post_warning("Thunderstorm Advisory.")
function post_warning(warning)
     
     --data = warning

     --print(data)

     socket = net.createConnection(net.TCP,0)
     socket:on("receive",display)
     socket:connect(80,SERVER_HOST)

     request = "POST "..WARNING_URI.." HTTP/1.1\r\n"..
          "Host: "..SERVER_HOST.."\r\n"..
          "Connection: close\r\n"..
          "Content-Type: application/x-www-form-urlencoded\r\n"..
          "Content-Length: "..string.len(warning).."\r\n"..
          "\r\n"..
          warning
     print(request)
     
     socket:on("connection",function(sck)
          sck:send(request)
     end)

     print("End of WARNING POST")
end

------- TECHTREK GET TRAIL RATING FROM AWS SERVER --------------

RATING_URL = "rating/firmware/1"

function get_rating()

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,SERVER_HOST)

    -- send get request
    socket:on("connection", function(socket)
          new_request = build_get_request(SERVER_HOST, RATING_URL)
          socket:send(new_request)
    end)

end

------- TECHTREK POST TRAIL RATING FROM AWS SERVER --------------

RATING_URI = "/rating/1"

-- Example: post_rating(1)
function post_rating(rate)
    
    data = ""

    socket = net.createConnection(net.TCP,0)
    socket:on("receive",display)
    socket:connect(80,SERVER_HOST)

    socket:on("connection",function(sck)
         post_request = build_rating_request(SERVER_HOST,RATING_URI,data, rate)
         sck:send(post_request)
    end)

end



-------------- Useful if you need to make a file -------------

---- This function registers a function to echo back any response from the server, to our DE1/NIOS system 
---- or hyper-terminal (depending on what the dongle is connected to)
--function display_weather(sck,response)
--     print(response)
--
--     file.remove(get_weather_file)
--     file.open(get_weather_file, "w")
--     file.write(response)
--     collectgarbage()
--     
--end
--
---- read_file("weather.txt")
--function read_file(filename)
--     if file.open(filename, "r") then
--   
--       print(file.read())
--       file.close()
--     end
--   end
