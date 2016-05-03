
package com.libPLUSandroid;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;

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

	/*
	 * Enum
	 * Video Stream enumerator
	 */
	public enum MediaStream { audioIn, audioOut, videoIn, videoOut, contentIn, contentOut, localVideoOut };

	/*
	 * Events
	 * Override these functions to receive events
	 */
    public void onstatus(String val1, String val2) {}
    public void onisinitialised(String val) {}
    public void onvideoframe(String val) {}
    public void oncallerid(String val) {}
    public void onincomingcall(String val) {}
    public void onincall(String val) {}
    public void onencryption(String val) {} 
    public void onfarEndCameraInstruct(String val) {}
    public void onrealTimeTextInstruct(String val) {}
    public void onfarEndMotoisedInstruct(String val) {}
    public void onPhoneNumberEvent(String val) {}
    public void onURIaddressEvent(String val) {}
    public void onpresenceSupport(String val) {}
    public void onduplicate(String notused) {}
    public void onforwardCall(String val) {}
    public void ondhGenerate(String val) {}
    public void onmediastart(String val1, String val2) {}
    // IMPL: Event Names here

	/*
	 * Media Out Events
	 */
    public void outaudio(byte[] val, int size) {}
    public void outvideo(byte[] val, int size) {}
    public void outcontent(byte[] val, int size) {}

	/*
	 * Methods
	 */
    public native void Load();
    public native void UnLoad();

    public native void doplaceCall(String p1);
    public native void dohangupCall();
    public native void doanswerCall();
    public native void dosendDTMF(String p1);
    public native void dofarEndCamera(String p1, String p2, String p3);
    public native void dostart();
    public native void dofarEndMotorized(String p1, String p2, String p3);
    public native void dorealTimeTextInput(String p1);
    public native void dorealTimeTextNewLine();
    public native void dosecondCall(String p1);
    public native void dostop();
    public native void dovideosize(String p1, String p2, String p3);
	public native void doaudioformat(String p1, String p2, String p3, String p4);
    public native void dodhParameters();
    // IMPL: Method Names Here

	/*
	 * Media In functions
	 */
    public native void doaudio(byte[] val, int size, int width, int height);
    public native void dovideo(byte[] val, int size, int width, int height);
    public native void docontent(byte[] val, int size, int width, int height);

	/*
	 * Settings
	 */
    public native String getversion();
    public native void setversion(String val);

    public native String gettracing();
    public native void settracing(String val);

    public native String getusername();
    public native void setusername(String val);

    public native String getpassword();
    public native void setpassword(String val);

    public native String getserver();
    public native void setserver(String val);

    public native String getquality();
    public native void setquality(String val);

    public native String getaccessability();
    public native void setaccessability(String val);

    public native String getcontent();
    public native void setcontent(String val);

    public native String getautoanswer();
    public native void setautoanswer(String val);

    public native String getdrvvideoplay();
    public native void setdrvvideoplay(String val);

    public native String getdrvvideorec();
    public native void setdrvvideorec(String val);

    public native String getdrvaudioplay();
    public native void setdrvaudioplay(String val);

    public native String getdrvaudiorec();
    public native void setdrvaudiorec(String val);

    public native String getcurdrvvideoplay();
    public native void setcurdrvvideoplay(String val);

    public native String getcurdrvvideorec();
    public native void setcurdrvvideorec(String val);

    public native String getcurdrvaudioplay();
    public native void setcurdrvaudioplay(String val);

    public native String getcurdrvaudiorec();
    public native void setdcurdrvaudiorec(String val);

    public native String getdevvideoplay();
    public native void setdevvideoplay(String val);

    public native String getdevvideorec();
    public native void setdevvideorec(String val);

    public native String getdevaudioplay();
	public native void setdevaudioplay(String val);

    public native String getdevaudiorec();
	public native void setdevaudiorec(String val);

    public native String getaudioplay();
    public native void setaudioplay(String val);

    public native String getaudiorec();
    public native void setaudiorec(String val);

    public native String getvideoplay();
    public native void setvideoplay(String val);

    public native String getvideorec();
    public native void setvideorec(String val);

    public native String getcall();
    public native void setcall(String val);

    public native String getaudiomute();
    public native void setaudiomute(String val);

    public native String getvideomute();
    public native void setvideomute(String val);

//    public native String getframewidth();
//    public native void setframewidth(String val);
//    public native String getframeheight();
//    public native void setframeheight(String val);

    public native String getcallstate();
    public native void setcallstate(String val);

    public native String getfarEndCameraSupport();
    public native void setfarEndCameraSupport(String val);

    public native String getrealTimeTextSupport();
    public native void setrealTimeTextSupport(String val);

    public native String getfarEndMotorizedSupport();
    public native void setfarEndMotorizedSupport(String val);

    public native String getfarEndCameraInCall();
    public native void setfarEndCameraInCall(String val);

    public native String getrealTimeTextInCall();
    public native void setrealTimeTextInCall(String val);

    public native String getfarEndMotorizedInCall();
    public native void setfarEndMotorizedInCall(String val);

    public native String getuserPhoneNumber();
    public native void setuserPhoneNumber(String val);

    public native String getuserURIAddress();
    public native void setuserURIAddress(String val);

    public native String getshowlocalvideo();
    public native void setshowlocalvideo(String val);

//    public native String getinstanceid();  - Not required
//    public native void setinstanceid(String val); // - Not required

    public native String getinitialised();
    public native void setinitialised(String val);

    public native String getlanguage();
    public native void setlanguage(String val);

    public native String getlistenport();
    public native void setlistenport(String val);

    public native String getvideoformats();
    public native void setvideoformats(String val);

    public native String getvideoinformat();
    public native void setvideoinformat(String val);

    public native String getvideooutformat();
    public native void setvideooutformat(String val);

    public native String getsecondvideo();
    public native void setsecondvideo(String val);

    public native void setencryptsignal(String val);
    public native String getencryptSignal();

    public native void setencryptmedia(String val);
    public native String getencryptMedia();

    public native String getencryptMediaHigh();
    public native void setencryptmediahigh(String val);
	// IMPL: Setting Names Here

   static {
    	try {
            System.loadLibrary("PLUSjni");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }

    String TestDisplay() {
	 String x = "Settings Iteration%n" +
				"version = " + getversion() + "%n" +
				"tracing = " + gettracing() + "%n" +
				"username = "  + getusername() + "%n" +
				"password = "  + getpassword() + "%n" +
				"server = "  + getserver() + "%n" +
				"quality = "  + getquality() + "%n" +
				"accessability = "  + getaccessability() + "%n" +
				"content = "  + getcontent() + "%n" +
				"autoanswer = "  + getautoanswer() + "%n" +
				"drvvideoplay= "  + getdrvvideoplay() + "%n" +
				"drvvideorec= "  + getdrvvideorec() + "%n" +
				"drvaudioplay= "  + getdrvaudioplay() + "%n" +
				"drvaudiorec= "  + getdrvaudiorec() + "%n" +
				"curdrvvideoplay= "  + getcurdrvvideoplay() + "%n" +
				"curdrvvideorec= "  + getcurdrvvideorec() + "%n" +
				"curdrvaudioplay= "  + getdevaudioplay() + "%n" +
				"curdrvaudiorec= "  + getdevaudiorec() + "%n" +
				"devvideoplay= "  + getdevvideoplay() + "%n" +
				"devvideorec= "  + getdevaudiorec() + "%n" +
				"devaudioplay= "  + getaudioplay() + "%n" +
				"devaudiorec= "  + getdevaudiorec() + "%n" +
				"audioplay= "  + getaudioplay() + "%n" +
				"audiorec= "  + getaudiorec() + "%n" +
				"videoplay= "  + getvideoplay() + "%n" +
				"videorec= "  + getvideorec() + "%n";
		return x;
    }

}
