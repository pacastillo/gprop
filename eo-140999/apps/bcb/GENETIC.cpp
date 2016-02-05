//----------------------------------------------------------------------------
//Borland C++Builder
//Copyright (c) 1987, 1997 Borland International Inc. All Rights Reserved.
//----------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
USERES("Genetic.res");
USEFORM("Unit2.cpp", Form2);
USEFORM("Unit3.cpp", Form3);
USEFORM("TGAViz.cpp", GAViz);
USEUNIT("\MyFiles\progs\eo\util\Random.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	Application->Initialize();
    Application->CreateForm(__classid(TGAViz), &GAViz);
		Application->Run();

	return 0;
}
//---------------------------------------------------------------------------
