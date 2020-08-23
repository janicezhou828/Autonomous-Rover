function [ mode ] = wallpattern( front, left, right, back )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
wall = 15;
counter = 0;

sensor = [front left right back];

for x = 1:1:size(sensor,1) % row
    for y = 1:1:size(sensor,2) %column
        if isnan(sensor(x,y)) % replace NaN with 0
            sensor(x,y) = 0;
        end
    end
end

for i = 1:1:4
    sensor_m(i) = median(sensor(:,i));
    if (sensor_m(i) < wall)
        counter = counter + 1;
    end
end

if (counter == 0)
    mode = 0;
elseif (counter == 1)
    mode = 1;
elseif (counter == 2)
    if ( (sensor_m(1) < wall && sensor_m(4) < wall) || (sensor_m(2) < wall && sensor_m(3) < wall))
        mode = 5;
    else
        mode = 2;
    end
elseif (counter == 3)
    mode = 3;
elseif (counter == 4)
    mode = 4;
    disp('Incorrect sensor reading. Re-evaluate surroundings.');
end

end

