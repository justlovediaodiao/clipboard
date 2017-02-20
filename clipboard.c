#include "stdio.h"
#include "windows.h"
#include "Python.h"

BOOL setclipboard(char* text)
{
	HGLOBAL h;
	LPVOID p;
	int size;
	//calc the num of unicode char
	size = MultiByteToWideChar(CP_UTF8, NULL, text, -1, NULL, 0);
	if(!size) return FALSE;
	h = GlobalAlloc(GHND|GMEM_SHARE, size * 2);
	if(!h) return FALSE;
	p = GlobalLock(h);
	//utf8 to unicode
	MultiByteToWideChar(CP_UTF8, NULL, text, -1, (LPWSTR)p, size);
	GlobalUnlock(h);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, h);
	CloseClipboard();
	return TRUE;
}

PyObject* clipboard_setclipboard(PyObject* self, PyObject* args)
{
	char* text;
	if(!PyArg_ParseTuple(args, "s", &text))
		return NULL;
	return Py_BuildValue("b", setclipboard(text));
}

PyMethodDef method[]=
{
	{ "setclipboard", clipboard_setclipboard, METH_VARARGS },
	{ NULL, NULL },
};

PyModuleDef module=
{
	PyModuleDef_HEAD_INIT,
	"clipboard",
	NULL,
	-1,
	method
};

PyMODINIT_FUNC PyInit_clipboard()
{
	return PyModule_Create(&module);
}

int main()
{
	char text[] = 
	{
		0x74,0x65,0x73,0x74,0x20,0x66,0x6f,0x72,0x20,0xe4,0xb8,0xad,0xe6,0x96,0x87, 0x00
	};
	setclipboard(text);
}