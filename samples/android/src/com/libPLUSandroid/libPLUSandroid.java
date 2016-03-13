
package com.libPLUSandroid;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;

import java.lang.reflect.*;

public class libPLUSandroid extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

		Load();

        /* Create a TextView and set its text to "Hello world" */
        TextView  tv = new TextView(this);
        tv.setText( String.format( TestDisplay() ) );
        setContentView(tv);
		
    }

    /** Called when the activity is destroyed. */
    @Override
	public void onDestroy() {

	   UnLoad();
	   super.onDestroy();

	 }

    String TestDisplay() {

	 String x = "Settings Iteration%n" +
				"version = " + version() + "%n" +
				"tracing = " + tracing() + "%n" +
				"username = " + username() + "%n" +
				"password = " + password() + "%n" +
				"server = " + server() + "%n" +
				"quality = " + quality() + "%n" +
				"accessability = " + accessability() + "%n" +
				"content = " + content() + "%n" +
				"autoanswer = " + autoanswer() + "%n" +
				"drvvideoplay= " + drvvideoplay() + "%n" +
				"drvvideorec= " + drvvideorec() + "%n" +
				"drvaudioplay= " + drvaudioplay() + "%n" +
				"drvaudiorec= " + drvaudiorec() + "%n" +
				"curdrvvideoplay= " + curdrvvideoplay() + "%n" +
				"curdrvvideorec= " + curdrvvideorec() + "%n" +
				"curdrvaudioplay= " + devvideoplay() + "%n" +
				"curdrvaudiorec= " + devvideorec() + "%n" +
				"devvideoplay= " + audioplay() + "%n" +
				"devvideorec= " + devaudiorec() + "%n" +
				"devaudioplay= " + audioplay() + "%n" +
				"devaudiorec= " + devaudiorec() + "%n" +
				"audioplay= " + audioplay() + "%n" +
				"audiorec= " + audiorec() + "%n" +
				"videoplay= " + videoplay() + "%n" +
				"videorec= " + videorec() + "%n";
		return x;
    }

    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */

    public native String stringFromJNI();

    public native void Load();
    public native void UnLoad();

    public native String version();
    public native String tracing();
    public native String username();
    public native String password();
    public native String server();
    public native String quality();
    public native String accessability();
    public native String content();
    public native String autoanswer();

    public native String drvvideoplay();
    public native String drvvideorec();
    public native String drvaudioplay();
    public native String drvaudiorec();
    public native String curdrvvideoplay();
    public native String curdrvvideorec();
    public native String curdrvaudioplay();
    public native String curdrvaudiorec();
    public native String devvideoplay();
    public native String devvideorec();
    public native String devaudioplay();
    public native String devaudiorec();
    public native String audioplay();
    public native String audiorec();
    public native String videoplay();
    public native String videorec();

    public native String call();
    public native String audiomute();
    public native String videomute();
//    public native String framewidth();
//    public native String frameheight();
    public native String callstate();
    public native String farEndCameraSupport();
    public native String realTimeTextSupport();
    public native String farEndMotorizedSupport();
    public native String farEndCameraInCall();
    public native String realTimeTextInCall();
    public native String farEndMotorizedInCall();
    public native String userPhoneNumber();
    public native String userURIAddress();
    public native String showlocalvideo();
//    public native String instanceid();  - Not required 
    public native String initialised();
    public native String language();
    public native String listenport();
    public native String encryptSignal();
    public native String encryptMedia();
    public native String encryptMediaHigh();

   static {
    	try {
            System.loadLibrary("PLUSjni");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }

}
