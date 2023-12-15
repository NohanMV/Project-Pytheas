// ThetaManager.cpp
#include "ThetaManager.h"

float UThetaManager::GetCameraTemperature()
{
    // Création de la requête HTTP
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetVerb("POST");
    Request->SetURL("URL_DE_L_API_RICOH_THETA/osc/state");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(TEXT("{}"));

    float Temperature = 0.0f;

    // Liaison de la fonction lambda pour gérer la réponse
    Request->OnProcessRequestComplete().BindLambda([this, &Temperature](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        if (bWasSuccessful && Response.IsValid())
        {
            // Désérialisation de la réponse JSON
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
            {
                // Récupération de la température depuis le champ '_boardTemp'
                int32 BoardTemp = 0;
                if (JsonObject->TryGetNumberField("_boardTemp", BoardTemp))
                {
                    // Conversion de la température selon la formule spécifiée
                    Temperature = static_cast<float>(BoardTemp * 9) / 10.0f;

                    // Informations de débogage
                    UE_LOG(LogTemp, Warning, TEXT("Température de la caméra récupérée avec succès : %f"), Temperature);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("La réponse de l'API ne contient pas le champ '_boardTemp'."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Impossible de désérialiser la réponse JSON de l'API."));
            }
        }
        else
        {
            // Gestion de l'échec de la requête HTTP
            UE_LOG(LogTemp, Error, TEXT("Échec de la requête HTTP vers l'API Ricoh Theta. Code de réponse : %d"), Response.IsValid() ? Response->GetResponseCode() : 0);
        }
    });

    // Envoi de la requête
    Request->ProcessRequest();

    return Temperature;
}