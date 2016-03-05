
package com.libPLUS_android;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;

public class libPLUS_android extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its text to "Hello world" */
        TextView  tv = new TextView(this);
        tv.setText("Hello World!");
        setContentView(tv);
    }

    static {
    	try {
            System.loadLibrary("PLUSjni");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }
}
