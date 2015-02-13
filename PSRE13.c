// Author : Matthew Watts

void process_input_file();

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char sVersionString[100] = "Import email web forms v1.3";
char sMattWattsEmail[100] = "m.watts@uq.edu.au";

// input file is a ascii email file of web forms
// we need to process the file and remove some non-ascii characters if file is a Pegasus file
// Unix files will be straight ascii so this is not necessary

char *process_record(char *sLine, char *sRecord)
{
     char *sVarVal;
     int iRecordLength, i;

     sVarVal = strstr(sLine,sRecord);

     if (sVarVal != NULL)
     {
        strcpy(sVarVal,sLine + strlen(sRecord));
        iRecordLength = strlen(sVarVal);

        for (i=0;i<iRecordLength;i++)  // filter out commas and end of line characters
        {
            if (sVarVal[i] == 44)
               sVarVal[i] = 32;
            if (sVarVal[i] < 32)
               sVarVal[i] = 0;
        }
    }
    return (sVarVal);
}

void process_input_file(char *sInputFile, char *sOutputFile)
{
    char *sInputFileName, *readname, *sVarVal;
    char sLine[5000],sLine2[5000],sLine3[5000],sRecordNumber[10];
    char sDate[1000], sIPAddress[1000], sRealname[1000], sOrganisation[1000];
    char sCountry[1000], sCountryOptions[1000], sEmail[1000], sSubscribe[1000];
    char sSubscribeDP[1000], sIndustry[1000], sIndustryOptions[1000];
    char sInterest[1000], sInterestOptions[1000], sSubject[1000];
    char sTitle[1000], sFirstname[1000], sLastname[1000], sPosition[1000];
    char sAddress1[1000], sAddress2[1000], sAddress3[1000], sPhone[1000], sComments[1000];
    FILE *fp_input_file, *fp_output_file;
    int iLinesInInputFile, iRecordNumber, iRecordLength, i, iUsers = 0, iLine;

    // create output
    readname = (char *) calloc(strlen(sOutputFile)+2, sizeof(char));
    strcpy(readname,sOutputFile);
    if ((fp_output_file = fopen(readname,"w"))==NULL)
    {
       printf("could not create output file %s\nAborting Program.",readname);
       exit(1);
    }
    free(readname);

    // open input file
    readname = (char *) calloc(strlen(sInputFile)+2, sizeof(char));
    strcpy(readname,sInputFile);
    if ((fp_input_file = fopen(readname,"r"))==NULL)
    {
       printf("could not open input file %s\nAborting Program.",readname);
       exit(1);
    }
    free(readname);

    fprintf(fp_output_file,"date,ipaddress,realname,organisation,country,countryoptions,email,subscribe,");
    fprintf(fp_output_file,"subscribeDP,industry,industryoptions,interest,interestoptions,subject,");
    fprintf(fp_output_file,"title,firstname,lastname,position,address1,address2,address3,phone,comments\n");

    iLine = 0;
    strcpy(sDate,"");
    strcpy(sIPAddress,"");
    strcpy(sRealname,"");
    strcpy(sOrganisation,"");
    strcpy(sCountry,"");
    strcpy(sCountryOptions,"");
    strcpy(sEmail,"");
    strcpy(sSubscribe,"");
    strcpy(sSubscribeDP,"");
    strcpy(sIndustry,"");
    strcpy(sIndustryOptions,"");
    strcpy(sInterest,"");
    strcpy(sInterestOptions,"");
    strcpy(sSubject,"");

    strcpy(sTitle,"");
    strcpy(sFirstname,"");
    strcpy(sLastname,"");
    strcpy(sPosition,"");
    strcpy(sAddress1,"");
    strcpy(sAddress2,"");
    strcpy(sAddress3,"");
    strcpy(sPhone,"");
    strcpy(sComments,"");

    while (fgets(sLine,5000-1,fp_input_file))
    {
          // software download parameters
          //"Date: "
          //"Subject: Mail form from "
          //"realname: "
          //"organisation: "
          //"country: "
          //"email: "
          //"subscribe: "
          //"subscribeDP: "
          //"industry: "
          //"interest: "
          //"subject: "

          // sTitle, sFirstname, sLastname, sPosition, sAddress1, sAddress2, sAddress3, sPhone, sComments
          // course enrolment parameters
          //"Title: "
          //"firstname: "
          //"lastname: "
          //"position: "
          //"address1: "
          //"address2: "
          //"address3: "
          //"phone: "
          //"comments: "
          //""


          iLine++;
          //if (iLine < 100)
          printf("processing line %i\n",iLine);

          // process the current line from input file
          if (strlen(sLine) > 0)
          if (strlen(sLine) < 200)
          {
             // scan for which records are present from the current email in the current line
             sVarVal = process_record(sLine,"Subject: Mail form from ");
             if (sVarVal != NULL)
             {
                //strcpy(sIPAddress,sVarVal);
                sprintf(sIPAddress,"%s",sVarVal);

                //if (iLine < 100)
                //   printf("varval >%s< ipaddress >%s< line %i\n",sVarVal,sIPAddress,iLine);
		     }

             sVarVal = process_record(sLine,"Date: ");
             if (sVarVal != NULL)
             {
                strcpy(sDate,sVarVal);
                iUsers++;

                //if (iLine < 100)
                //   printf("varval >%s< date >%s< line %i\n",sVarVal,sDate,iLine);
             }

             sVarVal = process_record(sLine,"realname: ");
             if (sVarVal != NULL)
                strcpy(sRealname,sVarVal);

             sVarVal = process_record(sLine,"organisation: ");
             if (sVarVal != NULL)
                strcpy(sOrganisation,sVarVal);

             sVarVal = process_record(sLine,"country: ");
             if (sVarVal != NULL)
                strcpy(sCountry,sVarVal);

             sVarVal = process_record(sLine,"country_options: ");
             if (sVarVal != NULL)
                strcpy(sCountryOptions,sVarVal);

             sVarVal = process_record(sLine,"email: ");
             if (sVarVal != NULL)
                strcpy(sEmail,sVarVal);

             sVarVal = process_record(sLine,"subscribe: ");
             if (sVarVal != NULL)
                strcpy(sSubscribe,sVarVal);

             sVarVal = process_record(sLine,"subscribeDP: ");
             if (sVarVal != NULL)
                strcpy(sSubscribeDP,sVarVal);

             sVarVal = process_record(sLine,"industry: ");
             if (sVarVal != NULL)
                strcpy(sIndustry,sVarVal);

             sVarVal = process_record(sLine,"industry_options: ");
             if (sVarVal != NULL)
                strcpy(sIndustryOptions,sVarVal);

             sVarVal = process_record(sLine,"interest: ");
             if (sVarVal != NULL)
                strcpy(sInterest,sVarVal);

             sVarVal = process_record(sLine,"research_interest_options: ");
             if (sVarVal != NULL)
                strcpy(sInterestOptions,sVarVal);

             sVarVal = process_record(sLine,"subject: ");
             if (sVarVal != NULL)
                strcpy(sSubject,sVarVal);

             // course enrolment parameters
             sVarVal = process_record(sLine,"Title: ");
             if (sVarVal != NULL)
                strcpy(sTitle,sVarVal);
             sVarVal = process_record(sLine,"firstname: ");
             if (sVarVal != NULL)
                strcpy(sFirstname,sVarVal);
             sVarVal = process_record(sLine,"lastname: ");
             if (sVarVal != NULL)
                strcpy(sLastname,sVarVal);
             sVarVal = process_record(sLine,"position: ");
             if (sVarVal != NULL)
                strcpy(sPosition,sVarVal);
             sVarVal = process_record(sLine,"address1: ");
             if (sVarVal != NULL)
                strcpy(sAddress1,sVarVal);
             sVarVal = process_record(sLine,"address2: ");
             if (sVarVal != NULL)
                strcpy(sAddress2,sVarVal);
             sVarVal = process_record(sLine,"address3: ");
             if (sVarVal != NULL)
                strcpy(sAddress3,sVarVal);
             sVarVal = process_record(sLine,"phone: ");
             if (sVarVal != NULL)
                strcpy(sPhone,sVarVal);
             sVarVal = process_record(sLine,"comments: ");
             if (sVarVal != NULL)
                strcpy(sComments,sVarVal);

             // if the current email is passed, write records for email to output table
             sVarVal = process_record(sLine,"From: ");
             if (sVarVal != NULL)
             {
                if (strlen(sDate) > 0)
                   fprintf(fp_output_file,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                                       sDate,sIPAddress,sRealname,sOrganisation,sCountry,
                                       sCountryOptions,sEmail,sSubscribe,sSubscribeDP,
                                       sIndustry,sIndustryOptions,sInterest,sInterestOptions,sSubject,
                                       sTitle,sFirstname,sLastname,sPosition,sAddress1,
                                       sAddress2,sAddress3,sPhone,sComments);
                strcpy(sDate,"");
                strcpy(sIPAddress,"");
                strcpy(sRealname,"");
                strcpy(sOrganisation,"");
                strcpy(sCountry,"");
                strcpy(sCountryOptions,"");
                strcpy(sEmail,"");
                strcpy(sSubscribe,"");
                strcpy(sSubscribeDP,"");
                strcpy(sIndustry,"");
                strcpy(sIndustryOptions,"");
                strcpy(sInterest,"");
                strcpy(sInterestOptions,"");
                strcpy(sSubject,"");
                strcpy(sTitle,"");
                strcpy(sFirstname,"");
                strcpy(sLastname,"");
                strcpy(sPosition,"");
                strcpy(sAddress1,"");
                strcpy(sAddress2,"");
                strcpy(sAddress3,"");
                strcpy(sPhone,"");
                strcpy(sComments,"");
			 }
          }
    }

    fprintf(fp_output_file,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                           sDate,sIPAddress,sRealname,sOrganisation,sCountry,
                           sCountryOptions,sEmail,sSubscribe,sSubscribeDP,
                           sIndustry,sIndustryOptions,sInterest,sInterestOptions,sSubject,
                           sTitle,sFirstname,sLastname,sPosition,sAddress1,
                           sAddress2,sAddress3,sPhone,sComments);

    // close files
    fclose(fp_output_file);
    fclose(fp_input_file);

    printf("\n%i emails processed\n",iUsers);

}

void display_usage(char *sExecutableFile);

void display_usage(char *sExecutableFile)
{
     // no arguments specified or incorrect number of arguments
     fprintf(stderr,"\n");
     fprintf(stderr,"Usage: %s [input file name] [output file name]\n\n",sExecutableFile);

     exit(1);
}

int main(int argc,char *argv[])
{
    printf("\n        %s \n\n",sVersionString);
    printf("   Coded by Matthew Watts\n");
    printf("   Written by Matthew Watts\n\n");
    printf("   %s\n\n",sMattWattsEmail);

    if (argc != 3)
       display_usage(argv[0]);
    else
    {
        process_input_file(argv[1],argv[2]);
    }

    return 0;
}
