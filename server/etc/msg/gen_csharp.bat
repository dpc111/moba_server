del proto_csharp\*.cs

for /f "delims=" %%i in ('dir /b /a-d /o-d "proto\*.proto"') do (
	echo %%i
    ..\tools\gen_csharp\protogen -i:proto\%%i -o:proto_csharp\%%i_tmp
)

cd proto_csharp\
ren *.proto_tmp *.cs
cd ..\

pause
