clear;clc;                                                                           %清除变量 
%串口初始化
g=serial('com5');                                                              %创建串口对象
g.baudrate=9600;                                                            %设置波特率,缺省9600bit/s
g.parity='none';                                                                 %无奇偶校验
g.stopbits=1;                                                                     %停止位
g.timeout=0.5;                                                                   %设置读操作完成时间为1s,缺省10s                                           
g.inputbuffersize=256;                                                     %输入缓冲区为32b，缺省值为512b
%设置
recbuf=zeros(40,14);                                                        %清接收缓冲区（40）        
framenum=1;                                                                     %清接收帧数
datanum=1;                                                                        %接收每帧数据字节数
statusFlag=0;                                                                     %接收计数器清零
checksum=0;                                                                     %校验位
%输入接收的数据帧数
recnum= input('请输入要接收的数据帧数:\n');                                 %输入接收帧数                                                                      %清  
%打开串口设备
fopen(g);                                                                  %打开串口设备对象g
%进入接收循环
while framenum<=recnum                                                      %
       recdta=fread(g,1,'uint8') ;                                         %读入数据
        if recdta==170&&statusFlag==0                                %找帧头1(0xAA)
            statusFlag=statusFlag+1;                                                   %接收计数器加1，继续寻找帧头2
        elseif recdta==187&&statusFlag==1                         %找帧头2(0xBB)
            statusFlag=statusFlag+1;                                                    %接收计数器加1，继续寻找帧长度
        elseif  statusFlag==2                                                     %找帧长度
            framelen=recdta;
            checksum=0;          
            checksum = bitxor(checksum,recdta);
            statusFlag=statusFlag+1;                                                    %接收计数器加1，继续寻找帧数据
        elseif statusFlag==3
            recbuf(framenum,datanum)=recdta;
            checksum = bitxor(checksum,recdta);
            datanum=datanum+1;
            if datanum>framelen
                statusFlag=statusFlag+1;
                datanum=1;
            end
        elseif statusFlag==4
            statusFlag=0;
            if checksum~=recdta
                 recbuf(framenum,16)=recdta;
                 recbuf(framenum,17)=checksum;
            end
            framenum=framenum+1; 
        end                                                              %接收结束   
end                                                                        %主循环结束                                                                                
%程序结束关闭串口类
fclose(g);                                                                 %关闭串口                                                                
delete(g);                                                                 %删除串口对象
clear g ;                                                                  %清除变量 
%将首字节转化为有符号数
for i=1:size(recbuf,1)
    for j=1:size(recbuf,2)
        if mod(j,2)==1&&recbuf(i,j)>127
            recbuf(i,j)=recbuf(i,j)-256;
        end
    end
end
for i=1:size(recbuf,1)
    for j=1:3
        acc(i,j) = recbuf(i,2*j-1)*256+recbuf(i,2*j);
    end
    temp(i,1) = recbuf(i,7)*256+recbuf(i,8);
    for j=1:3
        gyro(i,j) = recbuf(i,2*j+7)*256+recbuf(i,2*j+8);
    end
end
% save acc;
% save temp;
% save gyro;
% save recbuf;