#include <mysql/mysql.h>  //gcc时加`mysql_config --libs`
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
void  mysql_add(char *name,char *password)
{
	MYSQL *conn=mysql_init(NULL);
	if(mysql_real_connect(conn,
					//"iZ2ze0r9s4hc8s46tviov7Z",
					"localhost",
					"root",
					//"123456",
					"654321",
					//"L0904",
					"l0904",
					//8888,
					3306,
					NULL,0)==NULL)
  		printf("连接错误,信息%s,代码%d\n",mysql_error(conn),mysql_errno(conn));

		/*创建一个预处理声明变量 */
	MYSQL_STMT *statement=mysql_stmt_init(conn);

	//注册
	//数据库 用户名  密码（加密）

	/*创建预处理语句的公共部分，变动部分用?来占位
	 * insert into orders values(,,,) */
	char *str="insert into light values(?,?)";
	/*进行预处理，也就是将公共部分的语句先发给服务器 */
	mysql_stmt_prepare(statement,str,strlen(str));

	/*
	剩下的是sql语句的变动部分
	变动部分有两种类型：一种是输入型，是客户端发给服务器的值，叫做参数param。
	                    另外一种是输出型的，是服务器发过来的值，叫做结果result。
	*/
	/*将剩余2个参数进行设置 */
	/*创建用来保存2个参数的数据结构 ,是一个结构体*/
	MYSQL_BIND param[2];

	/*设置对应2个参数的C变量，用来保存数据 */
	bzero(param,sizeof(param));//清空

	//MD5加密  gcc时加-lcrypto
	MD5_CTX ctx;
	unsigned char md[16];
	char buf[33]={'\0'};//密码（加密）
	char tmp[3]={'\0'};
	MD5_Init(&ctx);
	MD5_Update(&ctx,password,strlen(password));
	MD5_Final(md,&ctx);
	int i;
	for( i=0; i<16; i++ )
	{
		sprintf(tmp,"%02X",md[i]);
		strcat(buf,tmp);
	}

	/*将2个变量和2个参数进行绑定 */
	param[0].buffer=name;//变量的地址
	param[0].buffer_length=strlen(name);//变量的长度
	param[0].buffer_type=MYSQL_TYPE_STRING;//变量的类型

	param[1].buffer=buf;//变量的地址
	param[1].buffer_length=strlen(buf);
	param[1].buffer_type=MYSQL_TYPE_STRING;

	/*将参数和预处理声明绑定 */
	mysql_stmt_bind_param(statement,param);

	/*将参数发给服务器 */
	mysql_stmt_execute(statement);

	//可以发送给客户端 提示注册成功（也可以没有）

//清空light表
//mysql> truncate light;

	/*将预处理声明关闭 */
	mysql_stmt_close(statement);
	mysql_close(conn);
}
	





int mysql_find(char *uname,char *upassword)
{
	//登陆
	//接收客户端输入的  用户名 密码 与数据库中的 进行比对
	MYSQL *conn=mysql_init(NULL);
	if(mysql_real_connect(conn,
					//"iZ2ze0r9s4hc8s46tviov7Z",
					"localhost",
					"root",
					//"123456",
					"654321",
					//"L0904",
					"l0904",
					//8888,
					3306,
					NULL,0)==NULL)
  		printf("连接错误,信息%s,代码%d\n",mysql_error(conn),mysql_errno(conn));

		/*创建一个预处理声明变量 */
	MYSQL_STMT *statement=mysql_stmt_init(conn);
	//md5(password)为小写  要加upper
	
	//用户名 密码与数据库中进行比对
	char *str="select* from light where  name=? && password=upper(md5(?)) ";
	/*进行预处理，也就是将公共部分的语句先发给服务器 */
	mysql_stmt_prepare(statement,str,strlen(str));
	MYSQL_BIND param[2];
	/*设置对应2个参数的C变量，用来保存数据 */
	bzero(param,sizeof(param));//清空

	param[0].buffer=uname;//变量的地址
	param[0].buffer_length=strlen(uname);//变量的长度
	param[0].buffer_type=MYSQL_TYPE_STRING;//变量的类型

	param[1].buffer=upassword;//变量的地址
	param[1].buffer_length=strlen(upassword);
	param[1].buffer_type=MYSQL_TYPE_STRING;

	mysql_stmt_bind_param(statement,param);

	char name[30];
	char password[100];//密码（未加密）

	MYSQL_BIND result[2];
	bzero(result,sizeof(result));//清空

	/*将2个变量和2个参数进行绑定 */
	result[0].buffer=name;//变量的地址
	result[0].buffer_length=sizeof(name);//变量的长度
	result[0].buffer_type=MYSQL_TYPE_STRING;//变量的类型

	result[1].buffer=password;
	result[1].buffer_length=sizeof(password);
	result[1].buffer_type=MYSQL_TYPE_STRING;

	/*将参数和预处理声明绑定 */
	mysql_stmt_bind_result(statement,result);

	/*从服务器获取数据 */
	mysql_stmt_execute(statement);
	
	// printf("uname   password:\n");
	/*将服务器发来的值显示出来 */
	if(mysql_stmt_fetch(statement)!=MYSQL_NO_DATA)
	{
		//printf("%s\t%s\n",name,password);
		return 1;//成功
	}
	else
	{
		//printf("error\n");
		return 0;
	}

	/*将预处理声明关闭 */
	mysql_stmt_close(statement);
	mysql_close(conn);
}




