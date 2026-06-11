// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString NewMessage)
{
	FText NewMessageText = FText::FromString(NewMessage);
	MessageTextBlock->SetText(NewMessageText);
}

