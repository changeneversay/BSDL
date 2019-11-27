# BSDL文件的解析代码
环境配置（visual studio）：

1
需要安装MySQL数据库，之后新建C++项目，

2
C:\Program Files\MySQL\MySQL Server 8.0\include 目录编辑到      工程----属性---C/C++---常规---附加包含目录

3 
C:\Program Files\MySQL\MySQL Server 8.0\lib 目录编辑到   项目--属性---链接器---常规---附加库目录

4 将lib文件夹中的libmysql.lib和libmysql.dll拷贝到本地项目文件夹中。


5   项目--属性---链接器---输入---附加依赖项 添加 libmysql.lib



注意：
1
connect_database.h与main.cpp文件中

#include <winsock.h>


#include <mysql.h>



#pragma comment(lib,"libmySQL.lib") 

//要加不然会编译出错，顺序不要错

2
如果仍然编译出错，请将配置管理器切换成X64/X86后，重新生成解决方案

3
请修改main.cpp文件中的inFile_BSDL（）括号内的文件路径为你需要解析的BSDL文件路径

请修改connect-database.cpp文件的Process_database函数内connect函数的参数值，将其改为自己的用户名与密码
