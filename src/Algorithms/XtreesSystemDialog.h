//
//  XtreesSystemDialog.h
//  Xtrees
//
//  Created by Kamen Dimitrov on 9/1/14.
//
//

#ifndef Xtrees_XtreesSystemDialog_h
#define Xtrees_XtreesSystemDialog_h

#include "ofMain.h"


ofFileDialogResult ofXTreesSystemLoadDialog(string windowTitle, bool bFolderSelection, string defaultPath);
ofFileDialogResult ofXTreesSystemSaveDialog(string defaultName, string messageName, string defaultPath);


#endif
