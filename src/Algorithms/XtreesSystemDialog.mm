//
//  XtreesSystemDialog.mm
//  Xtrees
//
//  Created by Kamen Dimitrov on 9/1/14.
//
//

#include "XtreesSystemDialog.h"


// OS specific results here.  "" = cancel or something bad like can't load, can't save, etc...
ofFileDialogResult ofXTreesSystemLoadDialog(string windowTitle, bool bFolderSelection, string defaultPath){
	
	ofFileDialogResult results;

	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSOpenPanel * loadDialog = [NSOpenPanel openPanel];
	[loadDialog setAllowsMultipleSelection:NO];
	[loadDialog setCanChooseDirectories:bFolderSelection];
	[loadDialog setResolvesAliases:YES];
	
	if(!windowTitle.empty()) {
		[loadDialog setTitle:[NSString stringWithUTF8String:windowTitle.c_str()]];
	}
	
	if(!defaultPath.empty()) {
		NSString * s = [NSString stringWithUTF8String:defaultPath.c_str()];
		s = [[s stringByExpandingTildeInPath] stringByResolvingSymlinksInPath];
		NSURL * defaultPathUrl = [NSURL fileURLWithPath:s];
		
		[loadDialog setDirectoryURL:defaultPathUrl];
	}
	
	NSInteger buttonClicked = [loadDialog runModal];
	
	NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
	if(appWindow) {
		[appWindow makeKeyAndOrderFront:nil];
	}
	if(buttonClicked == NSFileHandlingPanelOKButton) {
		NSURL * selectedFileURL = [[loadDialog URLs] objectAtIndex:0];
		results.filePath = string([[selectedFileURL path] UTF8String]);
	}
	[pool drain];

	if( results.filePath.length() > 0 ){
		results.bSuccess = true;
		results.fileName = ofFilePath::getFileName(results.filePath);
	}
	
	return results;
}


ofFileDialogResult ofXTreesSystemSaveDialog(string defaultName, string messageName, string defaultPath){
	
	ofFileDialogResult results;
	
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------       OSX
	//----------------------------------------------------------------------------------------
	
	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSSavePanel * saveDialog = [NSSavePanel savePanel];
	[saveDialog setMessage:[NSString stringWithUTF8String:messageName.c_str()]];
	[saveDialog setNameFieldStringValue:[NSString stringWithUTF8String:defaultName.c_str()]];
	[saveDialog setAllowedFileTypes:nil];
	
	if(!defaultPath.empty()) {
		NSString * s = [NSString stringWithUTF8String:defaultPath.c_str()];
		s = [[s stringByExpandingTildeInPath] stringByResolvingSymlinksInPath];
		NSURL * defaultPathUrl = [NSURL fileURLWithPath:s];
	
		[saveDialog setDirectoryURL:defaultPathUrl];
	}
	
	NSInteger buttonClicked = [saveDialog runModal];
	
	
	NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
	if(appWindow) {
		[appWindow makeKeyAndOrderFront:nil];
	}
	
	if(buttonClicked == NSFileHandlingPanelOKButton){
		results.filePath = string([[[saveDialog URL] path] UTF8String]);
	}
	[pool drain];
	
	
	
	if( results.filePath.length() > 0 ){
		results.bSuccess = true;
		results.fileName = ofFilePath::getFileName(results.filePath);
	}
	
	return results;
}