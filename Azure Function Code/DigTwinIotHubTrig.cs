using Azure;
using Azure.Core.Pipeline;
using Azure.DigitalTwins.Core;
using Azure.Identity;
using Microsoft.Extensions.Logging;
using Microsoft.Azure.Functions.Worker;
using Newtonsoft.Json.Linq;
using Azure.Messaging.EventHubs;
using System;
using System.Text;
using Microsoft.Extensions.DependencyInjection;
using System.Net.Http; // Add this using statement



namespace DigitalHUBTwin
{
    public class DigTwinIotHubTrig
    {
        string updatedQuery = "SELECT * FROM digitaltwins WHERE condition = 'value'";
        string query = "SELECT * FROM digitaltwins";
        private static readonly HttpClient httpClient = new HttpClient();
        private static string adtServiceUrl = Environment.GetEnvironmentVariable("ADT_SERVICE_URL");

        private readonly ILogger _logger;

        public DigTwinIotHubTrig(ILogger<DigTwinIotHubTrig> logger)
        {
            _logger = logger;
        }


        [Function(nameof(DigTwinIotHubTrig))]
        public async Task Run([EventHubTrigger("m2oaeventhub", Connection = "EventHubConnectionString")] EventData[] events)
        {
            //Authenticate with Digital Twins
            var credentials = new DefaultAzureCredential();
            DigitalTwinsClient client = new DigitalTwinsClient(
                new Uri(adtServiceUrl), credentials, new DigitalTwinsClientOptions
                { Transport = new HttpClientTransport(httpClient) });
            _logger.LogInformation($"ADT service client connection created.");

            foreach (EventData @event in events)
            {
                try
                {
                    string messageBody = Encoding.UTF8.GetString(@event.Body.ToArray());
                    JObject deviceMessage = JObject.Parse(messageBody);

                    // Ensure deviceMessage is not null
                    if (deviceMessage != null)
                    {
                        _logger.LogInformation($"Deviceissss::::{deviceMessage}");
                        var TT1_PLC1 = deviceMessage["TT1_PLC1"];
                        //var ST1_PLC1 = deviceMessage["ST1_PLC1"];
                        var FT1_PLC2 = deviceMessage["FT1_PLC2"];
                        var WT1_PLC2 = deviceMessage["WT1_PLC2"];
                        var TT2_PLC3 = deviceMessage["TT2_PLC3"];
                        var Fan = deviceMessage["Fan"];
                        var Stirrer = deviceMessage["Stirrer"];
                        var FC1 = deviceMessage["FC1"];
                        var LC1 = deviceMessage["LC1"];
                        var Pump1 = deviceMessage["Pump1"];
                        var Pump2 = deviceMessage["Pump2"];
                        var J1 = deviceMessage["J1"];




                        // Ensure temperature is not null
                        if (TT1_PLC1 != null)
                        {
                            _logger.LogInformation($"TT1_PLC1 is:{TT1_PLC1}");
                           // _logger.LogInformation($"ST1_PLC1 is:{ST1_PLC1}");
                            _logger.LogInformation($"FT1_PLC2 is:{FT1_PLC2}");
                            _logger.LogInformation($"WT1_PLC2 is:{WT1_PLC2}");
                            _logger.LogInformation($"TT2_PLC3 is:{TT2_PLC3}");
                            _logger.LogInformation($"Fan is:{Fan}");
                            _logger.LogInformation($"Stirrer is:{Stirrer}");
                            _logger.LogInformation($"FC1 is:{FC1}");
                            _logger.LogInformation($"LC1 is:{LC1}");
                            _logger.LogInformation($"Pump1 is:{Pump1}");
                            _logger.LogInformation($"Pump2 is:{Pump2}");
                            _logger.LogInformation($"J1 is:{J1}");



                            // Update twin using device temperature
                            var updateTwinMainTank = new JsonPatchDocument();
                            var updateTwinDataSubTank = new JsonPatchDocument();
                            var updateTwinDataRadiator = new JsonPatchDocument();
                            var updateTwinDataFlowTransmiter = new JsonPatchDocument();
                            var updateTwinDatacontrolvalve1 = new JsonPatchDocument();
                            var updateTwinDatacontrolvalve2 = new JsonPatchDocument();
                            var updateTwinDatapump1 = new JsonPatchDocument();
                            var updateTwinDatapump2 = new JsonPatchDocument();


                            updateTwinMainTank.AppendReplace("/TT1_PLC1", TT1_PLC1.Value<double>());
                            //updateTwinDataRadiator.AppendReplace("/ST1_PLC1", ST1_PLC1.Value<double>());
                            updateTwinDataFlowTransmiter.AppendReplace("/FT1_PLC2", FT1_PLC2.Value<double>());
                            updateTwinDataSubTank.AppendReplace("/WT1_PLC2", WT1_PLC2.Value<double>());
                            updateTwinMainTank.AppendReplace("/TT2_PLC3", TT2_PLC3.Value<double>());

                            updateTwinDataRadiator.AppendReplace("/Fan", Fan.Value<double>());
                            updateTwinMainTank.AppendReplace("/Stirrer", Stirrer.Value<double>());

                            updateTwinDatacontrolvalve1.AppendReplace("/FC1", FC1.Value<double>());
                            updateTwinDatacontrolvalve2.AppendReplace("/LC1", LC1.Value<double>());
                            updateTwinDatapump1.AppendReplace("/Pump1", Pump1.Value<double>());
                            updateTwinDatapump2.AppendReplace("/Pump2", Pump2.Value<double>());
                            updateTwinMainTank.AppendReplace("/J1", J1.Value<double>());



                            await client.UpdateDigitalTwinAsync("MainTank", updateTwinMainTank);
                            await client.UpdateDigitalTwinAsync("SubTank", updateTwinDataSubTank);
                            await client.UpdateDigitalTwinAsync("Radiator", updateTwinDataRadiator);
                            await client.UpdateDigitalTwinAsync("PressureTransmitterA", updateTwinDataFlowTransmiter);
                            await client.UpdateDigitalTwinAsync("ControlValveA", updateTwinDatacontrolvalve1);
                            await client.UpdateDigitalTwinAsync("ControlValveB", updateTwinDatacontrolvalve2);
                            await client.UpdateDigitalTwinAsync("PumpA", updateTwinDatapump1);
                            await client.UpdateDigitalTwinAsync("PumpB", updateTwinDatapump2);



                            _logger.LogInformation($"Updated Digital Twin for device: MY System M2OA");
                         
                        }



                        else
                        {
                            _logger.LogWarning("Data is null or invalid.");
                        }
                    }
                    else
                    {
                        _logger.LogWarning("Device message is null.");
                    }
                }
                catch (Exception ex)
                {
                    _logger.LogError($"Error processing message: {ex.Message}");
                }
            }
        }


    }
}