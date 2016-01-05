clear;clc;                                                                           %������� 
%���ڳ�ʼ��
g=serial('com5');                                                              %�������ڶ���
g.baudrate=9600;                                                            %���ò�����,ȱʡ9600bit/s
g.parity='none';                                                                 %����żУ��
g.stopbits=1;                                                                     %ֹͣλ
g.timeout=0.5;                                                                   %���ö��������ʱ��Ϊ1s,ȱʡ10s                                           
g.inputbuffersize=256;                                                     %���뻺����Ϊ32b��ȱʡֵΪ512b
%����
recbuf=zeros(40,14);                                                        %����ջ�������40��        
framenum=1;                                                                     %�����֡��
datanum=1;                                                                        %����ÿ֡�����ֽ���
statusFlag=0;                                                                     %���ռ���������
checksum=0;                                                                     %У��λ
%������յ�����֡��
recnum= input('������Ҫ���յ�����֡��:\n');                                 %�������֡��                                                                      %��  
%�򿪴����豸
fopen(g);                                                                  %�򿪴����豸����g
%�������ѭ��
while framenum<=recnum                                                      %
       recdta=fread(g,1,'uint8') ;                                         %��������
        if recdta==170&&statusFlag==0                                %��֡ͷ1(0xAA)
            statusFlag=statusFlag+1;                                                   %���ռ�������1������Ѱ��֡ͷ2
        elseif recdta==187&&statusFlag==1                         %��֡ͷ2(0xBB)
            statusFlag=statusFlag+1;                                                    %���ռ�������1������Ѱ��֡����
        elseif  statusFlag==2                                                     %��֡����
            framelen=recdta;
            checksum=0;          
            checksum = bitxor(checksum,recdta);
            statusFlag=statusFlag+1;                                                    %���ռ�������1������Ѱ��֡����
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
        end                                                              %���ս���   
end                                                                        %��ѭ������                                                                                
%��������رմ�����
fclose(g);                                                                 %�رմ���                                                                
delete(g);                                                                 %ɾ�����ڶ���
clear g ;                                                                  %������� 
%�����ֽ�ת��Ϊ�з�����
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
save acc;
save temp;
save gyro;
save recbuf;