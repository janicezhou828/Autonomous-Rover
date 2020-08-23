clear

% x_0 = instrfind;
% fclose(x_0);
% x=serial('/dev/cu.usbserial-1410','BAUD', 9600);

instrhwinfo('bluetooth'); % find bluetooth port
x = Bluetooth('HC-06',1); % create bluetooth port
fopen(x);

%variable initialization
go = true;
index = 1;

p1 = ones(4,8)*(1/32); 
p2 = ones(4,8)*(1/32); 
p3 = ones(4,8)*(1/32); 
p4 = ones(4,8)*(1/32); 

pmax1 = 0.001;
pmax2 = 0.001;
pmax3 = 0.001;
pmax4 = 0.001;

heading1 = 0;
heading2 = 90;
heading3 = 180;
heading4 = 270; %270 degrees implied down

pmax = max([pmax1, pmax2, pmax3, pmax4]);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%% Give "DATA" command to Arduino %%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
while pmax < 0.37 % value need to be finalized
    
    count = 1; % reset count after asking data for 3 times

    while count < 4 % use count to keep track of how many times matlab asks for data
        
        % a= input('\nPress 1, 2 and 3 to reset counter, start data transmission, and terminate program:');
        fprintf(x,'A');
        disp('DATA command sent to Arduino.');
        pause(2); % IMPORTANT pause 1 seconds to wait for incoming signal

        if (x.bytesAvailable > 0)
             sample=fscanf(x); %read what is shown on the COMPORT
             ind1 = find(sample =='!');
             ind2 = find(sample ==',');
             ind3 = find(sample ==';');
             ind4 = find(sample ==':');
             ind5 = find(sample =='?');
             ind6 = find(sample =='#');
             distance(count, 1)= str2double(sample(1:ind1-1)); %front
             distance(count, 2)= str2double(sample(ind1+1:ind2-1)); %back
             distance(count, 3)= str2double(sample(ind2+1:ind3-1)); %right front
             distance(count, 4)= str2double(sample(ind3+1:ind4-1)); %left front
             distance(count, 5)= str2double(sample(ind4+1:ind5-1)); %right back
             distance(count, 6)= str2double(sample(ind5+1:ind6-1)); %left back
             dir(count,1) = sample(ind6+1:ind6+1); %direction
             disp(distance);
             count = count +1;
        end
    end

m_u1(index) = wallpattern(distance(:,1), distance(:,4), distance(:,3), distance(:,2)); % take median, evaluate wall pattern and store in m_u 
m_m1(index) = median(dir(:,1)); % take median and store direction info in m_m

m_u2(index) = wallpattern(distance(:,1), distance(:,4), distance(:,3), distance(:,2)); % take median, evaluate wall pattern and store in m_u 
m_m2(index) = median(dir(:,1)); % take median and store direction info in m_m

m_u3(index) = wallpattern(distance(:,1), distance(:,4), distance(:,3), distance(:,2)); % take median, evaluate wall pattern and store in m_u 
m_m3(index) = median(dir(:,1)); % take median and store direction info in m_m

m_u4(index) = wallpattern(distance(:,1), distance(:,4), distance(:,3), distance(:,2)); % take median, evaluate wall pattern and store in m_u 
m_m4(index) = median(dir(:,1)); % take median and store direction info in m_m

%disp(m_u);
%disp(m_m);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%% LOCALIZATION STARTS %%%%%%%%%%%%%

%%%%%% initalization of the world %%%%%%%
dim1 = 8; dim2 = 4; 
locationindex = reshape(1:dim1*dim2,dim1,dim2)'; 
n = numel(locationindex);
rand('twister',5489);
% bw = reshape(randi([0 1],n,1),dim2,dim1); %0 = black, 1 = white
M = [1 1 1 1 0 1 0 1;1 1 0 1 1 1 1 1;1 0 1 0 0 1 0 1;1 1 1 1 1 1 0 1];
%%%%%% add maze boundary %%%%%%%
M = [zeros(dim2,1) M zeros(dim2,1)];
M = [zeros(1,dim1+2); M; zeros(1,dim1+2)];

% generate ultrasonic world
ultra = [2 1 5 2 0 3 0 3;1 2 0 2 5 0 5 1;5 0 3 0 0 5 0 5;2 5 1 5 5 2 0 3];

% create mask for blocks
M = M(2:end-1, 2:end-1);
%%%%%%%%%%%%%%%
subplot(2,2,1);
figure; imagesc(M); 
daspect([1 1 1])
title('Subplot 1: heading = 0')

subplot(2,2,2);
imagesc(M); 
daspect([1 1 1])
title('Subplot 2: heading = 90')

subplot(2,2,3);
imagesc(M); 
daspect([1 1 1])
title('Subplot 3 heading = 180')

subplot(2,2,4);
imagesc(M); 
daspect([1 1 1])
title('Subplot 4: heading = 270')

% figure; imagesc(M); 
% daspect([1 1 1])

% figure;
% localizatoin loop
% imagesc(p);

% m_u = [ 3, 1, 5, 0, 5];
% m_m = ['w','d','w','w','w'];

% possibility update based in sensor reading
p1 = sense_u(ultra, M, p1, m_u1(index));
%possibility update based on movement
[p1, heading1] = move(p1, M, heading1, m_m1(index));

p2 = sense_u(ultra, M, p2, m_u2(index));
[p2, heading2] = move(p2, M, heading2, m_m2(index));

p3 = sense_u(ultra, M, p3, m_u3(index));
[p3, heading3] = move(p3, M, heading3, m_m3(index));

p4 = sense_u(ultra, M, p4, m_u4(index));
[p4, heading4] = move(p4, M, heading4, m_m4(index));

%%%%%%%%%%%% LOCALIZATION ENDS %%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% clim = [0 0.37]; 
% imagesc(p,clim);
% colorbar
% title(['step: ' num2str(index)]);

%%%%%%%%%%%%%%%%%%%%
subplot(2,2,1);
clim = [0 0.37]; 
imagesc(p1,clim);
colorbar
title(['Subplot 1: heading = 0, step: ' num2str(index)])

subplot(2,2,2);
clim = [0 0.37]; 
imagesc(p2,clim);
colorbar
title(['Subplot 1: heading = 90, step: ' num2str(index)])

subplot(2,2,3);
clim = [0 0.37]; 
imagesc(p3,clim);
colorbar
title(['Subplot 1: heading = 180, step: ' num2str(index)])

subplot(2,2,4);
clim = [0 0.37]; 
imagesc(p4,clim);
colorbar
title(['Subplot 1: heading = 270, step: ' num2str(index)])

%%%%%%%%%%%

    [cmax1, rmax1] = max(p1);
    [pmax1, cind1] = max(cmax1);
    rind1 = rmax1(cind1);
    [cmax2, rmax2] = max(p2);
    [pmax2, cind2] = max(cmax2);
    rind2 = rmax2(cind2);
    [cmax3, rmax3] = max(p3);
    [pmax3, cind3] = max(cmax3);
    rind3 = rmax3(cind3);
    [cmax4, rmax4] = max(p4);
    [pmax4, cind4] = max(cmax4);
    rind4 = rmax4(cind4);
pmax = max([pmax1, pmax2, pmax3, pmax4]);
if (pmax == pmax1)
    disp(p1);
    disp(pmax);
    cind = cind1;
    rind = rind1;
    heading = heading1;
elseif (pmax == pmax2)
    disp(p2);
    disp(pmax);
    cind = cind2;
    rind = rind2;
    heading = heading2;
elseif (pmax == pmax3)
    disp(p3);
    disp(pmax);
    cind = cind3;
    rind = rind3;
    heading = heading3;
elseif (pmax == pmax4)
    disp(p4);
    disp(pmax);  
    cind = cind4;
    rind = rind4;
    heading = heading4;
end 


% calculate maximum possibility and its indices
% [cmax, rmax] = max(p);
% [pmax, cind] = max(cmax);
% rind = rmax(cind);
% disp(p);
% disp(pmax);

% reset matrix distance and dir
clear distance;
clear dir;

index = index + 1;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%% Give "MOVE" command to Arduino %%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fprintf(x,'B');
pause(10); % wait for robot to move to the next block
flushinput(x);
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%% Hardcoded Path to Loading Zone %%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

poInd = [rind cind];
pInd = int2str([rind cind]); %p max possibility index
% heading
entry = 0;

% % Blink the LED three times when localization is complete
% fprintf(x,'y');
% sample = fscanf(x);
% finish = str2double(sample);
% if (finish == 9)   
%     pause(1);
% end 

% Hard code to direct the robot to the loading zone 

if heading ~= 270 % adjust if heading is not 270
    if heading == 0
        adjust = 'r';
    elseif heading == 90
        adjust = ['r' 'r'];
    else 
        adjust = 'l';
    end
    
    for i=1:length(adjust)
        fprintf(x,adjust(i));
        finish1 = 100;
        pause(4);
        while finish1 ~= 9
            if (x.bytesavailable > 0)
                sample1 = fscanf(x);
                ind1_1 = find(sample1 =='!');
                finish1 = str2double(sample1(1:ind1_1-1));
                pause(1);
            end
        end
    end 
    
end 


heading = 270; % start with the same direction at any location 

switch pInd
    case int2str([4 8])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'r' 'm' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([3 8])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'r' 'n' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([2 8])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([1 8])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['n' 'r' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([2 7])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([4 6])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'r' 'n' 'n' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([3 6])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'r' 'n' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([2 6])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([1 6])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['n' 'r' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([4 5])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['l' 'g' 'l' 'n' 'n' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([2 5])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([4 4])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['l' 'g' 'l' 'n' 'n' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([2 4])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([1 4])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'z'];
        entry = 1;
        heading = 180;
    case int2str([4 3])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['l' 'g' 'l' 'n' 'n' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([3 3])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['g' 'r' 'g' 'r' 'g' 'r' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;;
    case int2str([1 3])
        disp('Attention: Loading Zone Ahead!!!')
        step = ['l' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;
    case int2str([4 2])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['l' 'g' 'l' 'n' 'n' 'l' 'g' 'r' 'g' 'l' 'z'];
        entry = 1;
        heading = 180;
    case int2str([2 2])
        disp('Already in the Loading Zone!')
        step = ['r' 'r' 'g' 'r' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;
    case int2str([1 2])
        disp('Already in the Loading Zone!')
        step = ['l' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;
    case int2str([4 1])
        disp('Attention: Loading Zone Ahead!!!');
        step = ['r' 'r' 'g' 'r' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;
    case int2str([3 1])
        disp('Attention: Loading Zone Ahead!!!')
        step = ['r' 'r' 'g' 'r' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;
    case int2str([2 1])
        disp('Already in the Loading Zone!')
        step = ['r' 'r' 'g' 'r' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;
    case int2str([1 1])
        disp('Already in the Loading Zone!')
        step = ['l' 'g' 'r' 'r' 'z'];
        entry = 1;
        heading = 180;
end


 for i=1:length(step)
    fprintf(x,step(i));
    finish2 = 100;
    pause(4);
    while finish2 ~= 9
        if (x.bytesavailable > 0)
            sample2 = fscanf(x);
            ind2_1 = find(sample2 =='!');
            finish2 = str2double(sample2(1:ind2_1-1));
            pause(1);
        end
    end
 end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%% Hardcoded Path to Dropoff Location %%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
deliverpoint = 1; %%%%% MANUAL INPUT REQUIRED %%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% entry = 1;
godropoff = false;
finish5 = 100;
pause(4);
flushinput(x);

while finish5 ~= 8 || finish5 ~= 7
    if (x.bytesavailable > 0)
    sample5 = fscanf(x);
    ind5_1 = find(sample5 =='!');
    finish5 = str2double(sample5(1:ind5_1-1));
    pause(1);
        if (finish5 == 8 || finish5 == 7)
            disp('Block has been picked up. En route to the dropoff location.');
            pause(1);
            break
        end
    end
end

if (finish5 == 8)
    out = ['g' 'l' 'g'];
elseif (finish5 == 7)
    out = ['r' 'g' 'l' 'g'];
end

for i=1:length(out)
    fprintf(x,out(i));
    finish6= 100;
    pause(4);
    while finish6 ~= 9
        if (x.bytesavailable > 0)
            sample6 = fscanf(x);
            ind6_1 = find(sample6 =='!');
            finish6 = str2double(sample6(1:ind6_1-1));
            pause(1);
            godropoff = true;
        end
    end
end 

if (godropoff == true)
    if deliverpoint==1
       step1= ['l' 'n' 'n' 'l' 'o' 'p' 'h' 'c'];
    elseif deliverpoint==2
       step1= ['l' 'g' 'l' 'n' 'n' 'o' 'p' 'h' 'c'];
    elseif deliverpoint==3
       step1= ['l' 'g' 'l' 'n' 'n' 'r' 'g' 'l' 'o' 'p' 'h' 'c'];
    elseif deliverpoint==4
       step1= ['l' 'g' 'l' 'n' 'n' 'r' 'g' 'r' 'n' 'o' 'p' 'h' 'c'];
    end   
end

for i=1:length(step1)
    fprintf(x,step1(i));
    finish3= 100;
    pause(4);
    while finish3 ~= 9
        if (x.bytesavailable > 0)
            sample3 = fscanf(x);
            ind3_1 = find(sample3 =='!');
            finish3 = str2double(sample3(1:ind3_1-1));
            pause(1);
        end
    end
end 

%%%%%%%%%%%% Hardcoded Path to Dropoff Location %%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
