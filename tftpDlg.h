
// tftpDlg.h: 头文件
//
#pragma warning(disable:4996)
#pragma once

#include"CDlgdown.h"
#include"CDlgup.h"
#include"CDlgabout.h"
#include"client.h"




// CtftpDlg 对话框
class CtftpDlg : public CDialogEx
{
// 构造
public:
	CtftpDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TFTP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	// 服务端和客户端的ip地址
	sockaddr_in serverAddr, clientAddr;
	// 客户端socket
	SOCKET sock;
	// ip地址长短
	unsigned int addr_len;
	unsigned long Opt = 1;
	double transByte, consumeTime;
	FILE* logFp;
	char logBuf[512];
	time_t rawTime;
	tm* info;
	CString out;
	CString temps;
	char tempc[500];
private:
	CTabCtrl tabyyy;
	CDlgup upyyy;
	CDlgdown downyyy;
	CDlgabout aboutyyy;
	
public:
	afx_msg void OnTcnSelchangeTabyyy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedAbout();
	//ip
	CEdit editlocalip;
	CEdit editfarip;
	CString localip;
	CString farip;
	CString GetLocalIp()
	{
		editlocalip.GetWindowTextW(localip);
		return localip;
	}
	CString GetFarIp()
	{
		editfarip.GetWindowTextW(farip);
		return farip;
	}
	
	char filename[200];
	
	CString filenames;
	CString filenamefroms, filenametos;
	int charLen;
	int len;
	TCHAR* buftemp;
	int reupnum;
	int redownnum;

	CButton netascii_radio;
	CButton octet_radio;

	afx_msg void OnBnClickedLink();

	void LinkIp(char* clientIP, char* serverIP)
	{
		time(&rawTime);
		// 转化为当地时间
		info = localtime(&rawTime);
		// 打开日志文件
		logFp = fopen("tftpyyy.log", "a");
		if (logFp == NULL) {

			temps = L"打开日志文件失败！\r\n";
			printout = printout+temps;
			UpdateData(FALSE);
			temps.Empty();

		}
		temps.Empty();
		sprintf_s(tempc,"本地当前日期和时间：%s", asctime(info));
		int charLen = strlen(tempc);
		//计算多字节字符的大小，按字符计算。
		int len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
		//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
		TCHAR* buftemp = new TCHAR[len + 1];
		//多字节编码转换成宽字节编码
		MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
		buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
		//将TCHAR数组转换为CString
		temps.Append(buftemp);
		//删除缓冲区
		delete[]buftemp;
		printout = printout + temps + L"\r\n";
		UpdateData(FALSE);

		// 用于分割命令
		int nRC;
		WSADATA wsaData;
		addr_len = sizeof(struct sockaddr_in);
		//初始化 winsock
		nRC = WSAStartup(0x0101, &wsaData);
		if (nRC)
		{
			temps = L"客户端初始化winsock错误!\r\n";
			printout = printout + temps;
			UpdateData(FALSE);
			return;
		}
		if (wsaData.wVersion != 0x0101)
		{
			temps = L"客户端winsock版本错误!\r\n";
			printout = printout + temps;
			UpdateData(FALSE);
			WSACleanup();
			return;
		}

		temps = L"客户端winsock已初始化!\r\n";
		printout = printout + temps;
		UpdateData(FALSE);

		// 设置服务器 地址族(ipv4) 端口 ip
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(69);
		serverAddr.sin_addr.S_un.S_addr = inet_addr(serverIP);
		// 设置客户端 地址族(ipv4) 端口 ip
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_port = htons(0);
		clientAddr.sin_addr.S_un.S_addr = inet_addr(clientIP);
		// 创建客户端socket
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		ioctlsocket(sock, FIONBIO, &Opt);

		if (sock == INVALID_SOCKET)
		{
			// 创建失败
			temps = L"客户端创建socket错误!\r\n";
			printout = printout + temps;
			UpdateData(FALSE);

			WSACleanup();
			return;
		}

		temps = L"客户端创建socket成功!\r\n";
		printout = printout + temps;
		UpdateData(FALSE);

		nRC = bind(sock, (LPSOCKADDR)&clientAddr, sizeof(clientAddr));

		if (nRC == SOCKET_ERROR)
		{
			// 绑定失败
			temps = L"客户端绑定socket失败!\r\n";
			printout = printout + temps;
			UpdateData(FALSE);

			closesocket(sock);
			WSACleanup();
			return;
		}

		temps = L"客户端绑定socket成功!\r\n";
		printout = printout + temps;
		UpdateData(FALSE);
		temps.Empty();
	}

	char* GetFilename(char* p)
	{
		int x = strlen(p);
		char ch = '\\';
		char* q = strrchr(p, ch) + 1;
		if (strrchr(p, ch) == NULL)
			return p;
		else
			return q;
	}
	

	void UpLoad()
	{
		//CDlgup* temp_UPLOAD = (CDlgup*)AfxGetMainWnd();
		//MessageBox(filenames);

		reupnum = 0;

		USES_CONVERSION;

		char* filepathname = W2A(filenames);
		
		memset(filename, 0, sizeof(filename));

		strcpy(filename ,GetFilename(filepathname));



		// 记录时间
		clock_t start, end;
		// 传输字节数
		transByte = 0;
		// 用于保存服务器发送的ip和端口
		sockaddr_in sender;
		// 等待时间,接收包大小
		int time_wait_ack, r_size, resent = 0;
		// 发送包,接收包
		tftpPacket sendPacket, rcv_packet;
		// 写请求数据包
		// 写入操作码
		sendPacket.cmd = htons(CMD_WRQ);
		// 写入文件名以及传输格式
		int mode = octet_radio.GetCheck();
		//0是netascii,1是octet
		if (mode == 0)
		{
			sprintf(sendPacket.filename, "%s%c%s%c", filename, 0, "netascii", 0);
		}
		else
		{
			sprintf(sendPacket.filename, "%s%c%s%c", filename, 0, "octet", 0);
		}
		sendto(sock, (char*)&sendPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&serverAddr, addr_len);
		for (time_wait_ack = 0; time_wait_ack < PKT_RCV_TIMEOUT; time_wait_ack += 20) {
			// 非阻塞模式
			//ioctlsocket(sock, FIONBIO, &Opt);
			// 尝试接收
			r_size = recvfrom(sock, (char*)&rcv_packet, sizeof(tftpPacket), 0, (struct sockaddr*)&sender, (int*)&addr_len);
			if (r_size > 0 && r_size < 4) {
				// 接收包异常
				temps.Empty();
				sprintf_s(tempc,"Bad packet: r_size=%d", r_size);
				int charLen = strlen(tempc);
				//计算多字节字符的大小，按字符计算。
				int len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
				//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
				TCHAR* buftemp = new TCHAR[len + 1];
				//多字节编码转换成宽字节编码
				MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
				buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
				//将TCHAR数组转换为CString
				temps.Append(buftemp);
				//删除缓冲区
				delete[]buftemp;
				printout = printout + temps + L"\r\n";
				UpdateData(FALSE);
	
			}
			if (r_size >= 4 && rcv_packet.cmd == htons(CMD_ACK) && rcv_packet.block == htons(0)) {
				// 成功收到ACK
				break;
			}
			Sleep(20);
		}

		if (time_wait_ack >= PKT_RCV_TIMEOUT) {
			// 超时,未接受到ACK
			temps = L"接收不到服务器响应!\r\n";
			printout = printout + temps;
			UpdateData(FALSE);
			// 写入日志
			// 获取时间
			time(&rawTime);
			// 转化为当地时间
			info = localtime(&rawTime);
			sprintf(logBuf, "%s ERROR: upload %s, mode: %s, %s\n",
				asctime(info), filename, mode == 0 ? ("netascii") : ("octet"),
				"接收不到服务器响应.");
			for (int i = 0; i < 512; i++) {
				if (logBuf[i] == '\n') {
					logBuf[i] = ' ';
					break;
				}
			}
			fwrite(logBuf, 1, strlen(logBuf), logFp);
			return;
		}

		FILE* fp = NULL;
		if (mode == 0)
			fp = fopen(filepathname, "r");
		else
			fp = fopen(filepathname, "rb");
		if (fp == NULL) {

			temps = L"文件不存在!\r\n";
			printout = printout + temps;
			UpdateData(FALSE);
			// 写入日志
			// 获取时间
			time(&rawTime);
			// 转化为当地时间
			info = localtime(&rawTime);
			sprintf(logBuf, "%s ERROR: upload %s, mode: %s, %s\n",
				asctime(info), filepathname, mode == 0 ? ("netascii") : ("octet"),
				"文件不存在!");
			for (int i = 0; i < 512; i++) {
				if (logBuf[i] == '\n') {
					logBuf[i] = ' ';
					break;
				}
			}
			fwrite(logBuf, 1, strlen(logBuf), logFp);
			return ;
		}
		int s_size = 0;
		int rxmt;
		unsigned short block = 1;
		// 数据包操作码
		sendPacket.cmd = htons(CMD_DATA);
		// Send data.
		start = clock();

		do {
			// 清空数据
			memset(sendPacket.data, 0, sizeof(sendPacket.data));
			// 写入块编号
			sendPacket.block = htons(block);
			// 读入数据
			s_size = fread(sendPacket.data, 1, DATA_SIZE, fp);
			transByte += s_size;
			// 最多重传3次
			for (rxmt = 0; rxmt < PKT_MAX_RXMT; rxmt++) {
				// 发送数据包
				sendto(sock, (char*)&sendPacket, s_size + 4, 0, (struct sockaddr*)&sender, addr_len);
				temps.Empty();
				sprintf_s(tempc, "发送第%d个包", block);
				int charLen = strlen(tempc);
				//计算多字节字符的大小，按字符计算。
				int len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
				//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
				TCHAR* buftemp = new TCHAR[len + 1];
				//多字节编码转换成宽字节编码
				MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
				buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
				//将TCHAR数组转换为CString
				temps.Append(buftemp);
				//删除缓冲区
				delete[]buftemp;
				printout = printout + temps + L"\r\n";
				UpdateData(FALSE);

				// 等待ACK,最多等待3s,每隔20ms刷新一次
				for (time_wait_ack = 0; time_wait_ack < PKT_RCV_TIMEOUT; time_wait_ack += 20) {
					r_size = recvfrom(sock, (char*)&rcv_packet, sizeof(tftpPacket), 0, (struct sockaddr*)&sender, (int*)&addr_len);
					if (r_size > 0 && r_size < 4) {
						temps.Empty();
						sprintf_s(tempc, "Bad packet: r_size=%d", r_size);
						int charLen = strlen(tempc);
						//计算多字节字符的大小，按字符计算。
						int len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
						//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
						TCHAR* buftemp = new TCHAR[len + 1];
						//多字节编码转换成宽字节编码
						MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
						buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
						//将TCHAR数组转换为CString
						temps.Append(buftemp);
						//删除缓冲区
						delete[]buftemp;
						printout = printout + temps + L"\r\n";
						UpdateData(FALSE);
					}
					if (r_size >= 4 && rcv_packet.cmd == htons(CMD_ACK) && rcv_packet.block == htons(block)) {
						break;
					}
					Sleep(20);
				}
				if (time_wait_ack < PKT_RCV_TIMEOUT) {
					// 发送成功
					break;
				}
				else {
					// 未收到ACK，重传
					// 写入日志
					// 获取时间
					reupnum++;
					time(&rawTime);
					// 转化为当地时间
					info = localtime(&rawTime);
					sprintf(logBuf, "%s WARN: upload %s, mode: %s, %s\n",
						asctime(info), filepathname, mode == 0 ? ("netascii") : ("octet"),
						"无法接收ACK，重传");
					for (int i = 0; i < 512; i++) {
						if (logBuf[i] == '\n') {
							logBuf[i] = ' ';
							break;
						}
					}
					fwrite(logBuf, 1, strlen(logBuf), logFp);

					continue;
				}
			}
			if (rxmt >= PKT_MAX_RXMT) {
				// 3次重传失败

				temps = L"无法从服务器接收信息!\r\n";
				printout = printout + temps;
				UpdateData(FALSE);
				
				fclose(fp);

				// 写入日志
				// 获取时间
				time(&rawTime);
				// 转化为当地时间
				info = localtime(&rawTime);
				sprintf(logBuf, "%s ERROR: upload %s, mode: %s, %s\n",
					asctime(info), filepathname, mode == 0 ? ("netascii") : ("octet"),
					"无法从服务器接收信息!");
				for (int i = 0; i < 512; i++) {
					if (logBuf[i] == '\n') {
						logBuf[i] = ' ';
						break;
					}
				}
				fwrite(logBuf, 1, strlen(logBuf), logFp);

				return ;
			}
			// 传输下一个数据块
			block++;
		} while (s_size == DATA_SIZE);	// 当数据块未装满时认为时最后一块数据，结束循环

		end = clock();
		
		temps = L"文件发送结束!\r\n";
		printout = printout + temps;
		UpdateData(FALSE);

		fclose(fp);
		consumeTime = ((double)(end - start)) / CLK_TCK;
		temps.Empty();
		sprintf_s(tempc, "上传文件大小: %.1f kB  花费时间: %.2f s", transByte / 1024, consumeTime);
		charLen = strlen(tempc);
		//计算多字节字符的大小，按字符计算。
		len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
		//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
		buftemp = new TCHAR[len + 1];
		//多字节编码转换成宽字节编码
		MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
		buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
		//将TCHAR数组转换为CString
		temps.Append(buftemp);
		//删除缓冲区
		delete[]buftemp;
		printout = printout + temps + L"\r\n";
		UpdateData(FALSE);

		
		temps.Empty();
		sprintf_s(tempc, "上传速度: %.1f kB/s", transByte / (1024 * consumeTime));
		charLen = strlen(tempc);
		//计算多字节字符的大小，按字符计算。
		len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
		//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
		buftemp = new TCHAR[len + 1];
		//多字节编码转换成宽字节编码
		MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
		buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
		//将TCHAR数组转换为CString
		temps.Append(buftemp);
		//删除缓冲区
		delete[]buftemp;
		printout = printout + temps + L"\r\n";
		UpdateData(FALSE);


		temps.Empty();
		sprintf_s(tempc, "重传文件次数: %d  丢包率: %.2f%%", reupnum, 100.0 * reupnum / block);
		charLen = strlen(tempc);
		//计算多字节字符的大小，按字符计算。
		len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
		//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
		buftemp = new TCHAR[len + 1];
		//多字节编码转换成宽字节编码
		MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
		buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
		//将TCHAR数组转换为CString
		temps.Append(buftemp);
		//删除缓冲区
		delete[]buftemp;
		printout = printout + temps + L"\r\n";
		UpdateData(FALSE);

		// 获取时间
		time(&rawTime);
		// 转化为当地时间
		info = localtime(&rawTime);
		sprintf(logBuf, "%s INFO: upload %s, mode: %s, 大小: %.1f kB, 花费时间: %.2f s\n",
			asctime(info), filepathname, mode == 0 ? ("netascii") : ("octet"), transByte / 1024, consumeTime);
		for (int i = 0; i < 512; i++) {
			if (logBuf[i] == '\n') {
				logBuf[i] = ' ';
				break;
			}
		}
		fwrite(logBuf, 1, strlen(logBuf), logFp);
		return ;

	}




	void DownLoad()
	{
		USES_CONVERSION;
		
		char* remoteFile = W2A(filenamefroms);
		char* localFile = W2A(filenametos);

		// 记录时间
		clock_t start, end;
		// 传输字节数
		transByte = 0;
		// 用于保存服务器发送的ip和端口
		sockaddr_in sender;
		// 等待时间,接收包大小
		int r_size;
		// 发送包,接收包
		tftpPacket sendPacket, rcv_packet;

		int next_block = 1;
		int total_bytes = 0;

		int time_wait_data;
		unsigned short block = 1;

		// 读请求数据包
		// 读取操作码
		sendPacket.cmd = htons(CMD_RRQ);

		int mode = octet_radio.GetCheck();
		if (mode == 0)
			sprintf(sendPacket.filename, "%s%c%s%c", remoteFile, 0, "netascii", 0);
		else
			sprintf(sendPacket.filename, "%s%c%s%c", remoteFile, 0, "octet", 0);

		sendto(sock, (char*)&sendPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&serverAddr, addr_len);

		FILE* fp = NULL;
		if (mode == 0)
			fp = fopen(localFile, "w");
		else
			fp = fopen(localFile, "wb");
		if (fp == NULL) {

			temps.Empty();
			sprintf_s(tempc, "创建文件 \"%s\" 失败!\n", localFile);
			charLen = strlen(tempc);
			//计算多字节字符的大小，按字符计算。
			len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
			//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
			buftemp = new TCHAR[len + 1];
			//多字节编码转换成宽字节编码
			MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
			buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
			//将TCHAR数组转换为CString
			temps.Append(buftemp);
			//删除缓冲区
			delete[]buftemp;
			printout = printout + temps + L"\r\n";
			UpdateData(FALSE);

			// 获取时间
			time(&rawTime);
			// 转化为当地时间
			info = localtime(&rawTime);
			sprintf(logBuf, "%s ERROR: download %s as %s, mode: %s, 创建文件 \"%s\" 失败.\n",
				asctime(info), remoteFile, localFile, mode == 0 ? ("netascii") : ("octet"),
				localFile);
			for (int i = 0; i < 512; i++) {
				if (logBuf[i] == '\n') {
					logBuf[i] = ' ';
					break;
				}
			}
			fwrite(logBuf, 1, strlen(logBuf), logFp);

			return ;
		}

		start = clock();
		sendPacket.cmd = htons(CMD_ACK);
		do {
			for (time_wait_data = 0; time_wait_data < PKT_RCV_TIMEOUT * PKT_MAX_RXMT; time_wait_data += 10) {
				// Try receive(Nonblock receive).
				r_size = recvfrom(sock, (char*)&rcv_packet, sizeof(tftpPacket), 0, (struct sockaddr*)&sender, (int*)&addr_len);
				if (r_size > 0 && r_size < 4) {

					temps.Empty();
					sprintf_s(tempc, "Bad packet: r_size=%d\n", r_size);
					charLen = strlen(tempc);
					//计算多字节字符的大小，按字符计算。
					len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
					//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
					buftemp = new TCHAR[len + 1];
					//多字节编码转换成宽字节编码
					MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
					buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
					//将TCHAR数组转换为CString
					temps.Append(buftemp);
					//删除缓冲区
					delete[]buftemp;
					printout = printout + temps + L"\r\n";
					UpdateData(FALSE);

				}
				if (r_size >= 4 && rcv_packet.cmd == htons(CMD_DATA) && rcv_packet.block == htons(block)) {
					
					
					temps.Empty();
					sprintf_s(tempc, "数据: 数据块=%d, 数据大小=%d\n", ntohs(rcv_packet.block), r_size - 4);
					charLen = strlen(tempc);
					//计算多字节字符的大小，按字符计算。
					len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
					//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
					buftemp = new TCHAR[len + 1];
					//多字节编码转换成宽字节编码
					MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
					buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
					//将TCHAR数组转换为CString
					temps.Append(buftemp);
					//删除缓冲区
					delete[]buftemp;
					printout = printout + temps + L"\r\n";
					UpdateData(FALSE);
					// Send ACK.
					sendPacket.block = rcv_packet.block;
					sendto(sock, (char*)&sendPacket, sizeof(tftpPacket), 0, (struct sockaddr*)&sender, addr_len);
					fwrite(rcv_packet.data, 1, r_size - 4, fp);
					break;

				}
				Sleep(10);
			}
			transByte += (r_size - 4);
			if (time_wait_data >= PKT_RCV_TIMEOUT * PKT_MAX_RXMT) {


				temps.Empty();
				sprintf_s(tempc, "等待第 %d 块数据超时!\n", block);
				charLen = strlen(tempc);
				//计算多字节字符的大小，按字符计算。
				len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
				//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
				buftemp = new TCHAR[len + 1];
				//多字节编码转换成宽字节编码
				MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
				buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
				//将TCHAR数组转换为CString
				temps.Append(buftemp);
				//删除缓冲区
				delete[]buftemp;
				printout = printout + temps + L"\r\n";
				UpdateData(FALSE);

				fclose(fp);

				// 获取时间
				time(&rawTime);
				// 转化为当地时间
				info = localtime(&rawTime);
				sprintf(logBuf, "%s ERROR: download %s as %s, mode: %s, 等待第%d块数据超时.\n",
					asctime(info), remoteFile, localFile, mode == 0 ? ("netascii") : ("octet"),
					block);
				for (int i = 0; i < 512; i++) {
					if (logBuf[i] == '\n') {
						logBuf[i] = ' ';
						break;
					}
				}
				fwrite(logBuf, 1, strlen(logBuf), logFp);

				return ;
			}
			block++;
		} while (r_size == DATA_SIZE + 4);
		end = clock();
		consumeTime = ((double)(end - start)) / CLK_TCK;

		temps.Empty();
		sprintf_s(tempc, "下载文件大小: %.1f kB 消耗时间: %.2f s\n", transByte / 1024, consumeTime);
		charLen = strlen(tempc);
		//计算多字节字符的大小，按字符计算。
		len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
		//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
		buftemp = new TCHAR[len + 1];
		//多字节编码转换成宽字节编码
		MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
		buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
		//将TCHAR数组转换为CString
		temps.Append(buftemp);
		//删除缓冲区
		delete[]buftemp;
		printout = printout + temps + L"\r\n";
		UpdateData(FALSE);

		temps.Empty();
		sprintf_s(tempc, "下载速度: %.1f kB/s\n", transByte / (1024 * consumeTime));
		charLen = strlen(tempc);
		//计算多字节字符的大小，按字符计算。
		len = MultiByteToWideChar(CP_ACP, 0, tempc, charLen, NULL, 0);
		//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
		buftemp = new TCHAR[len + 1];
		//多字节编码转换成宽字节编码
		MultiByteToWideChar(CP_ACP, 0, tempc, charLen, buftemp, len);
		buftemp[len] = '\0'; //添加字符串结尾，注意不是len+1
		//将TCHAR数组转换为CString
		temps.Append(buftemp);
		//删除缓冲区
		delete[]buftemp;
		printout = printout + temps + L"\r\n";
		UpdateData(FALSE);

		fclose(fp);
		// 获取时间
		time(&rawTime);
		// 转化为当地时间
		info = localtime(&rawTime);
		sprintf(logBuf, "%s INFO: download %s as %s, mode: %s, 大小: %.1f kB, 消耗时间: %.2f s\n",
			asctime(info), remoteFile, localFile, mode == 0 ? ("netascii") : ("octet"), transByte / 1024, consumeTime);
		for (int i = 0; i < 512; i++) {
			if (logBuf[i] == '\n') {
				logBuf[i] = ' ';
				break;
			}
		}
		fwrite(logBuf, 1, strlen(logBuf), logFp);
		return ;

	}

	CEdit print;
	CString printout;
};
