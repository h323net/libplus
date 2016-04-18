using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using libplus;

namespace libPLUS_CSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            libPLUS plus = new libPLUS();
            plus.isinitialised += plus_isinitialised;
            plus.status += plus_status;
            plus.callerid += plus_callerid;
            plus.duplicate += plus_duplicate;
            plus.encryption += plus_encryption;
            plus.incall += plus_incall;
            plus.incomingcall += plus_incomingcall;
            plus.dhGenerate += plus_dhGenerate;

            // Load the libPLUS stack
            plus.Load();

            // Initialisation Settings
            plus.tracing = "6";
            plus.curdrvvideorec = "DirectShow";
            plus.curdrvvideoplay = "Window";
            plus.curdrvaudiorec = "WindowsMultimedia";
            plus.curdrvaudioplay = "WindowsMultimedia";
            plus.encryptMediaHigh = "1";
            plus.autoanswer = "1";
            plus.quality = "3";
 

            Console.WriteLine("Properties");
            foreach (var prop in plus.GetType().GetProperties())
            {
                Console.WriteLine("{0}={1}", prop.Name, prop.GetValue(plus, null));
            }

            // Start the libPLUS application
            plus.start();

            bool shutDown = false;

            while (!shutDown)
            {
                String a = Console.ReadLine().ToLower();

                if (a.Substring(0, 1) == "x") {
                    shutDown = true;
                    continue;
                }

                if (a.Substring(0, 1) == "c") 
                    plus.placeCall(a.Substring(2));
  
            }

            // Stop the libPLUS application
            plus.stop();

            // Unload the libPLUS stack
            plus.Unload(); 
            plus = null;
        }

        private static void plus_incomingcall(string p1)
        {
            Console.WriteLine("Incoming call: {0}", p1);
        }

        private static void plus_incall(string p1)
        {
            Console.WriteLine("InCall: {0}", p1);
        }

        private static void plus_farEndCameraInstruct(string p1)
        {
            throw new NotImplementedException();
        }

        private static void plus_encryption(string p1)
        {
            Console.WriteLine("Encryption: {0}", p1);
        }

        private static void plus_duplicate()
        {
            Console.WriteLine("Duplicate: ");
        }

        private static void plus_callerid(string p1)
        {
            Console.WriteLine("CallerID: {0}", p1);
        }

        private static void plus_status(string p1)
        {
            Console.WriteLine("Status: {0}", p1);
        }

        private static void plus_isinitialised(string p1)
        {
            Console.WriteLine("Initialised: {0}", p1);
        }

        private static void plus_dhGenerate(string p1)
        {
            Console.WriteLine("DH Generate: {0}", p1);
        }
    }
}
