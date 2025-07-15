// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessFactory.h"
#include "ChessActor.h"

UChessFactory::UChessFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UChessData::StaticClass();
	bCreateNew = true;
}

UObject* UChessFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {

	return NewObject<UChessData>(InParent, Class, Name, Flags);
}