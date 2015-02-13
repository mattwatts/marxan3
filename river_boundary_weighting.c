// Author : Matthew Watts
// Date : 6th August 2007
// Purpose : create weighted river boundary length file for Marxan

// compiler Optimization options

// -O2	Generate fastest possible code

typedef struct boundstuff
{
    int id1;
    int id2;
    double boundary;

} typeboundstuff;

typedef struct weightstuff
{
    int id;
    double weighting;

} typeweightstuff;

typedef struct computestuff
{
	int id1;
	int id2;
	double boundary;
	double rboundary;
	double wboundary;
	double weight1;
	double weight2;

} typecomputestuff;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int iSearchCount=0, iExtraDebug = 0;
char sVersionString[100] = "River boundary weightings v1.0";
char sMattWattsEmail[100] = "m.watts@uq.edu.au";

void WriteWeightingFile(int iWeightings,struct weightstuff weightings[])
{
     FILE *fp;
     char *writename;
     int i;

     writename = (char *) calloc(strlen("debug_weightings.csv") + 2, sizeof(char));
     strcpy(writename,"debug_weightings.csv");
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create weightings file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id,weighting\n");

     for (i=0;i<iWeightings;i++)
     {
         fprintf(fp,"%d,%lf\n",weightings[i].id,weightings[i].weighting);
     }

    fclose(fp);
}

int CreateSortedWeightings(int iWeightings,struct weightstuff weightings[],struct weightstuff *sortedweightings[])
{
	 int i, iAlreadySorted = 1, iPreviousId = -1;

     *sortedweightings = (struct weightstuff *) calloc(iWeightings,sizeof(struct weightstuff));

     for (i=0;i<iWeightings;i++)
     {
		(*sortedweightings)[i].id = weightings[i].id;
		(*sortedweightings)[i].weighting = weightings[i].weighting;

		if (weightings[i].id < iPreviousId)
		   iAlreadySorted = 0;
		iPreviousId = weightings[i].id;
     }

     return(iAlreadySorted);
}

void siftDown_weightings(struct weightstuff numbers[], int root, int bottom, int array_size)
{
  int done, maxChild;
  typeweightstuff temp;

  done = 0;
  while ((root*2 <= bottom) && (!done))
  {
   if (root*2 < array_size)
   {
    if (root*2 == bottom)
      maxChild = root * 2;
    else if (numbers[root * 2].id > numbers[root * 2 + 1].id)
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;

    if (numbers[root].id < numbers[maxChild].id)
    {
      temp = numbers[root];
      numbers[root] = numbers[maxChild];
      numbers[maxChild] = temp;
      root = maxChild;
    }
    else
      done = 1;
   }
   else
       done = 1;
  }
}

void heapSort_weightings(struct weightstuff numbers[], int array_size)
{
     int i;
     typeweightstuff temp;

     for (i = (array_size / 2)-1; i >= 0; i--)
         siftDown_weightings(numbers, i, array_size, array_size);

     for (i = array_size-1; i >= 1; i--)
     {
         temp = numbers[0];
         numbers[0] = numbers[i];
         numbers[i] = temp;
         siftDown_weightings(numbers, 0, i-1, array_size);
     }
}

void WriteSortedWeightings(int iWeightings,struct weightstuff sortedweightings[])
{
     FILE *fp;
     char *writename;
     int i;

     writename = (char *) calloc(strlen("debug_sortedweightings.csv") + 2, sizeof(char));
     strcpy(writename,"debug_sortedweightings.csv");
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create sorted weightings file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id,weighting\n");

     for (i=0;i<iWeightings;i++)
     {
         fprintf(fp,"%d,%lf\n",sortedweightings[i].id,sortedweightings[i].weighting);
     }

     fclose(fp);
}

int LookupWeightings(int iWeightings,int iId, struct weightstuff sortedweightings[])
{
    // use a binary search to find the index of iId1
    int iTop, iBottom, iCentre, iCount;

    iSearchCount++;
    iTop = 0;
    iBottom = iWeightings-1;
    iCentre = iTop + floor(iWeightings / 2);

    while ((iTop <= iBottom) && (sortedweightings[iCentre].id != iId))
    {
        if (iId < sortedweightings[iCentre].id)
        {
            iBottom = iCentre - 1;
            iCount = iBottom - iTop + 1;
            iCentre = iTop + floor(iCount / 2);
        }
        else
        {
            iTop = iCentre + 1;
            iCount = iBottom - iTop + 1;
            iCentre = iTop + floor(iCount / 2);
        }
    }
    if (sortedweightings[iCentre].id == iId)
       return(iCentre);
    else
        return(-1);
}

void CreateComputeStuff(int iRiverBoundaries,struct computestuff *computed[],struct boundstuff riverboundaries[])
{
	 int i;

     *computed = (struct computestuff *) calloc(iRiverBoundaries,sizeof(struct computestuff));

     for (i=0;i<iRiverBoundaries;i++)
     {
		(*computed)[i].id1 = riverboundaries[i].id1;
		(*computed)[i].id2 = riverboundaries[i].id2;
		(*computed)[i].rboundary = riverboundaries[i].boundary;
		(*computed)[i].boundary = 0;
		(*computed)[i].wboundary = 0;
		(*computed)[i].weight1 = 0;
		(*computed)[i].weight2 = 0;
	 }
}

int LookupId1(int iBoundaries,int iId1, struct boundstuff idindex[])
{
    // use a binary search to find the index of iId1
    int iTop, iBottom, iCentre, iCount;

    iSearchCount++;
    iTop = 0;
    iBottom = iBoundaries-1;
    iCentre = iTop + floor(iBoundaries / 2);

    while ((iTop <= iBottom) && (idindex[iCentre].id1 != iId1))
    {
        if (iId1 < idindex[iCentre].id1)
        {
            iBottom = iCentre - 1;
            iCount = iBottom - iTop + 1;
            iCentre = iTop + floor(iCount / 2);
        }
        else
        {
            iTop = iCentre + 1;
            iCount = iBottom - iTop + 1;
            iCentre = iTop + floor(iCount / 2);
        }
    }
    if (idindex[iCentre].id1 == iId1)
       return(iCentre);
    else
        return(-1);
}

int FindStartMatchId1(int iBoundaries,int iId1, struct boundstuff id1_index[],int *iMatchCount)
{
	int iMatch, iStart, iEnd, iSearch;

	if (iExtraDebug)
	   printf("before LookupId1 iId1 %i\n",iId1);

	iMatch = LookupId1(iBoundaries,iId1,id1_index);

	if (iExtraDebug)
	   printf("after LookupId1 iMatch %i\n",iMatch);

    if (iMatch == -1)
    {
	   *iMatchCount = 0;
	   return(-1);
	}
    else
    {
        iStart = iMatch;
        iEnd = iMatch;

	    if (iExtraDebug)
	       printf("before search back\n");

        // go backwards in array to find start of match
        iSearch = 1;
        while (iSearch == 1)
        {
			  if (iStart > 0)
			  {
				 if (id1_index[iStart-1].id1 == iId1)
				 	iStart--;
				 else
				     iSearch = 0;
			  }
			  else
			      iSearch = 0;
		}

	    if (iExtraDebug)
	       printf("before search forward iStart %i\n",iStart);

        // go forwards in array to find end of match
        iSearch = 1;
        while (iSearch == 1)
        {
			  if (iEnd < (iBoundaries-1))
			  {
				 if (id1_index[iEnd+1].id1 == iId1)
				 	iEnd++;
				 else
				 	 iSearch = 0;
			  }
			  else
			      iSearch = 0;

	          if (iExtraDebug)
	             printf("iEnd %i\n",iEnd);
		}

	    if (iExtraDebug)
	       printf("end find matches iEnd %i\n",iEnd);

		*iMatchCount = iEnd - iStart + 1;
		return(iStart);
    }
}

void ComputeWeightedBoundaries(int iRiverBoundaries,int iBoundaries,int iWeightings,struct computestuff computed[],
                               struct boundstuff idindex[],struct weightstuff sortedweightings[],
                               char sOutputFileName[])
{
     int i, j, iIdStart, iIdCount, iMatchFound, iWeightingIndex;
     FILE *fp;
     char *writename;

     writename = (char *) calloc(strlen(sOutputFileName) + 2, sizeof(char));
     strcpy(writename,sOutputFileName);
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create compute stuff file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id1,id2,boundary\n");

     for (i=0;i<iRiverBoundaries;i++)
     {
		 iMatchFound = 0;

		 iWeightingIndex = LookupWeightings(iWeightings,computed[i].id1,sortedweightings);

		 if (iWeightingIndex == -1)
		 {
			WriteSortedWeightings(iWeightings,sortedweightings);
            printf("STOP ERROR: cannot find weighting for id %i in sortedweightings\n",computed[i].id1);
            fclose(fp);
			exit(1);
		 }

		 computed[i].weight1 = sortedweightings[iWeightingIndex].weighting;

		 if (iExtraDebug)
		    printf("weight 1 set\n");

         iWeightingIndex = LookupWeightings(iWeightings,computed[i].id2,sortedweightings);

		 if (iWeightingIndex == -1)
		 {
			WriteSortedWeightings(iWeightings,sortedweightings);
            printf("STOP ERROR: cannot find weighting for id %i in sortedweightings\n",computed[i].id2);
            fclose(fp);
			exit(1);
		 }

		 computed[i].weight2 = sortedweightings[iWeightingIndex].weighting;

		 if (iExtraDebug)
		    printf("weight 2 set\n");

         iIdStart = FindStartMatchId1(iBoundaries,computed[i].id1,idindex,&iIdCount);
         if (iIdCount > 0)
         {
			for (j=iIdStart;j<(iIdStart+iIdCount);j++)
			    if (idindex[j].id2 == computed[i].id2)
			    {
				   // match found
				   iMatchFound = 1;
				   computed[i].boundary = idindex[j].boundary;

		           if (iExtraDebug)
		              printf("id1 match found id1 %i id2 %i boundary %lf\n",
		                     idindex[j].id1,idindex[j].id2,idindex[j].boundary);
				}
		 }

		 if (iExtraDebug)
		    printf("id1 search done\n");

		 if (iMatchFound == 0)
		 {
		    if (iExtraDebug)
		       printf("id1 match not found\n");

            iIdStart = FindStartMatchId1(iBoundaries,computed[i].id2,idindex,&iIdCount);
            if (iIdCount > 0)
            {
			   for (j=iIdStart;j<(iIdStart+iIdCount);j++)
			       if (idindex[j].id2 == computed[i].id1)
			       {
				      // match found
				      iMatchFound = 1;
				      computed[i].boundary = idindex[j].boundary;

		              if (iExtraDebug)
		                 printf("id2 match found id1 %i id2 %i boundary %lf\n",
		                        idindex[j].id1,idindex[j].id2,idindex[j].boundary);
				   }
		    }
		 }

		 if (iExtraDebug)
		 {
		    printf("id2 search done\n");

            printf("STOP\n");
            fclose(fp);
		    exit(1);
	     }

		 if (iMatchFound == 0)
		 {
            //
            printf("STOP ERROR: cannot find id pair %i,%i in boundary file\n",computed[i].id1,
                                                                              computed[i].id2);
			exit(1);

            fclose(fp);
		 }

		 computed[i].wboundary = computed[i].boundary * computed[i].weight1 * computed[i].weight2;

		 if (iExtraDebug)
		    printf("wboundary computed\n");

         fprintf(fp,"%d,%d,%lf\n",computed[i].id1,computed[i].id2,computed[i].wboundary);
	 }

     fclose(fp);
}

void WriteComputeStuff(int iRiverBoundaries,struct computestuff computed[])
{
     FILE *fp;
     char *writename;
     int i;

     writename = (char *) calloc(strlen("debug_computestuff.csv") + 2, sizeof(char));
     strcpy(writename,"debug_computestuff.csv");
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create compute stuff file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id1,id2,rboundary,boundary,weight1,weight2,wboundary\n");

     for (i=0;i<iRiverBoundaries;i++)
     {
         fprintf(fp,"%d,%d,%lf,%lf,%lf,%lf,%lf\n",computed[i].id1,computed[i].id2,computed[i].rboundary,
                    computed[i].boundary,computed[i].weight1,computed[i].weight2,computed[i].wboundary);
     }

     fclose(fp);
}

void siftDown_id1(struct boundstuff numbers[], int root, int bottom, int array_size)
{
  int done, maxChild;
  typeboundstuff temp;

  done = 0;
  while ((root*2 <= bottom) && (!done))
  {
   if (root*2 < array_size)
   {
    if (root*2 == bottom)
      maxChild = root * 2;
    else if (numbers[root * 2].id1 > numbers[root * 2 + 1].id1)
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;

    if (numbers[root].id1 < numbers[maxChild].id1)
    {
      temp = numbers[root];
      numbers[root] = numbers[maxChild];
      numbers[maxChild] = temp;
      root = maxChild;
    }
    else
      done = 1;
   }
   else
       done = 1;
  }
}

void heapSort_id1(struct boundstuff numbers[], int array_size)
{
     int i;
     typeboundstuff temp;

     for (i = (array_size / 2)-1; i >= 0; i--)
         siftDown_id1(numbers, i, array_size, array_size);

     for (i = array_size-1; i >= 1; i--)
     {
         temp = numbers[0];
         numbers[0] = numbers[i];
         numbers[i] = temp;
         siftDown_id1(numbers, 0, i-1, array_size);
     }
}

int CreateIdIndex(int iBoundaries,struct boundstuff boundaries[],struct boundstuff *idindex[])
{
	 int i, iAlreadySorted = 1, iPreviousId = -1;

     *idindex = (struct boundstuff *) calloc(iBoundaries,sizeof(struct boundstuff));

     for (i=0;i<iBoundaries;i++)
     {
		(*idindex)[i].id1 = boundaries[i].id1;
		(*idindex)[i].id2 = boundaries[i].id2;
		(*idindex)[i].boundary = boundaries[i].boundary;

		if (boundaries[i].id1 < iPreviousId)
		   iAlreadySorted = 0;
		iPreviousId = boundaries[i].id1;
     }

     return(iAlreadySorted);
}

void WriteId1(int iBoundaries,struct boundstuff id1index[])
{
     FILE *fp;
     char *writename;
     int i;

     writename = (char *) calloc(strlen("debug_id1.csv") + 2, sizeof(char));
     strcpy(writename,"debug_id1.csv");
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create id1 index file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id1,id2,boundary\n");

     for (i=0;i<iBoundaries;i++)
     {
         fprintf(fp,"%d,%d,%lf\n",id1index[i].id1,id1index[i].id2,id1index[i].boundary);
     }

     fclose(fp);
}

void WriteId1_end(int iBoundaries,struct boundstuff id1index[])
{
     FILE *fp;
     char *writename;
     int i;

     writename = (char *) calloc(strlen("debug_id1_end.csv") + 2, sizeof(char));
     strcpy(writename,"debug_id1_end.csv");
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create id1 index file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id1,id2,boundary\n");

     for (i=0;i<iBoundaries;i++)
     {
         fprintf(fp,"%d,%d,%lf\n",id1index[i].id1,id1index[i].id2,id1index[i].boundary);
     }

     fclose(fp);
}

void ReadBoundaryFile(char sBoundaryFileName[],int *iBoundaries,struct boundstuff *boundaries[])
{
     FILE *fp;
     char *readname, sLine[1000], *sVarVal;
     int i=0;

     readname = (char *) calloc(strlen(sBoundaryFileName) + 2, sizeof(char));

     strcpy(readname,sBoundaryFileName);
     if ((fp = fopen(readname,"r"))==NULL)
     {
        printf("boundary file %s has not been found.\nAborting Program.",sBoundaryFileName);
        exit(1);
	 }
     free(readname);

     fgets(sLine,999,fp);

     while (fgets(sLine,999,fp))
           i++;

     rewind(fp);
     fgets(sLine,999,fp);

     *iBoundaries = i;
     *boundaries = (struct boundstuff *) calloc(*iBoundaries,sizeof(struct boundstuff));

     for (i=0;i<*iBoundaries;i++)
     {
		fgets(sLine,999,fp);

        sVarVal = strtok(sLine," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%d", &(*boundaries)[i].id1);

        sVarVal = strtok(NULL," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%d", &(*boundaries)[i].id2);

        sVarVal = strtok(NULL," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%lf", &(*boundaries)[i].boundary);
    }
}

void WriteBoundaryFile(int iBoundaries,struct boundstuff boundaries[])
{
     FILE *fp;
     char *writename;
     int i;

     writename = (char *) calloc(strlen("debug_boundaries.csv") + 2, sizeof(char));
     strcpy(writename,"debug_boundaries.csv");
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create boundaries file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id1,id2,boundary\n");

     for (i=0;i<iBoundaries;i++)
     {
         fprintf(fp,"%d,%d,%lf\n",boundaries[i].id1,boundaries[i].id2,boundaries[i].boundary);
     }

    fclose(fp);
}

void ReadRiverBoundaryFile(char sRiverBoundaryFileName[],int *iRiverBoundaries,struct boundstuff *riverboundaries[])
{
     FILE *fp;
     char *readname, sLine[1000], *sVarVal;
     int i=0;

     readname = (char *) calloc(strlen(sRiverBoundaryFileName) + 2, sizeof(char));

     strcpy(readname,sRiverBoundaryFileName);
     if ((fp = fopen(readname,"r"))==NULL)
     {
        printf("river boundary file %s has not been found.\nAborting Program.",sRiverBoundaryFileName);
        exit(1);
	 }
     free(readname);

     fgets(sLine,999,fp);

     while (fgets(sLine,999,fp))
           i++;

     rewind(fp);
     fgets(sLine,999,fp);

     *iRiverBoundaries = i;
     *riverboundaries = (struct boundstuff *) calloc(*iRiverBoundaries,sizeof(struct boundstuff));

     for (i=0;i<*iRiverBoundaries;i++)
     {
		fgets(sLine,999,fp);

        sVarVal = strtok(sLine," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%d", &(*riverboundaries)[i].id1);

        sVarVal = strtok(NULL," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%d", &(*riverboundaries)[i].id2);

        sVarVal = strtok(NULL," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%lf", &(*riverboundaries)[i].boundary);
    }
}

void WriteRiverBoundaryFile(int iRiverBoundaries,struct boundstuff riverboundaries[])
{
     FILE *fp;
     char *writename;
     int i;

     writename = (char *) calloc(strlen("debug_riverboundaries.csv") + 2, sizeof(char));
     strcpy(writename,"debug_riverboundaries.csv");
     fp = fopen(writename,"w");
     if (fp==NULL)
     {
        printf("cannot create river boundaries file %s\n",writename);
        exit(1);
	 }
     free(writename);

     fprintf(fp,"id1,id2,boundary\n");

     for (i=0;i<iRiverBoundaries;i++)
     {
         fprintf(fp,"%d,%d,%lf\n",riverboundaries[i].id1,riverboundaries[i].id2,riverboundaries[i].boundary);
     }

    fclose(fp);
}

void ReadWeightingFile(char sWeightingFileName[],int *iWeightings,struct weightstuff *weightings[])
{
     FILE *fp;
     char *readname, sLine[1000], *sVarVal;
     int i=0;

     readname = (char *) calloc(strlen(sWeightingFileName) + 2, sizeof(char));

     strcpy(readname,sWeightingFileName);
     if ((fp = fopen(readname,"r"))==NULL)
     {
        printf("weighting file %s has not been found.\nAborting Program.",sWeightingFileName);
        exit(1);
	 }
     free(readname);

     fgets(sLine,999,fp);

     while (fgets(sLine,999,fp))
           i++;

     rewind(fp);
     fgets(sLine,999,fp);

     *iWeightings = i;
     *weightings = (struct weightstuff *) calloc(*iWeightings,sizeof(struct weightstuff));

     for (i=0;i<*iWeightings;i++)
     {
		fgets(sLine,999,fp);

        sVarVal = strtok(sLine," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%d", &(*weightings)[i].id);

        sVarVal = strtok(NULL," ,;:^*\"/|\t\'\\\n");
        sscanf(sVarVal, "%lf", &(*weightings)[i].weighting);
    }
}

int main(int argc,char *argv[])
{
	char sBoundaryFileName[100], sRiverBoundaryFileName[100], sWeightingFileName[100], sOutputFileName[100];
    int iBoundaries, iRiverBoundaries, iWeightings, iAlreadySorted, iDebug = 0;
    typeboundstuff *boundaries,*riverboundaries, *id1index;
    typeweightstuff *weightings, *sortedweightings;
    typecomputestuff *computed;

    printf("\n        %s \n\n",sVersionString);
    printf("   Coded by Matthew Watts\n");
    printf("   Written by Matthew Watts\n\n");
    printf("   %s\n\n",sMattWattsEmail);

	if (argc < 5)
	{
       // no arguments specified or incorrect number of arguments
       fprintf(stderr,"\nMarxan river connections v1.0\n\n");
       fprintf(stderr,"Usage: river_boundary_weighting [boundary file name]\n");
       fprintf(stderr,"                                [river boundary file name]\n");
       fprintf(stderr,"                                [weighting file name]\n");
       fprintf(stderr,"                                [output file name]\n\n");

       fprintf(stderr,"[boundary file] has fields id1,id2,boundary\n\n");
       fprintf(stderr,"[river boundary file] has fields id1,id2,river boundary\n\n");
       fprintf(stderr,"[weighting file] has fields id,weighting\n\n");
       fprintf(stderr,"[output file] has fields id1,id2,boundary\n");
       fprintf(stderr,"where boundary = boundary * weighting1 * weighting2\n\n");

       exit(1);
    }
	else
	{
		// handle the program options
	    strcpy(sBoundaryFileName,argv[1]);
	    strcpy(sRiverBoundaryFileName,argv[2]);
	    strcpy(sWeightingFileName,argv[3]);
	    strcpy(sOutputFileName,argv[4]);
    }

    printf("\n");
    if (iDebug == 1)
       printf("reading boundaries\n");

    ReadBoundaryFile(sBoundaryFileName,&iBoundaries,&boundaries);

    if (iDebug == 1)
       WriteBoundaryFile(iBoundaries,boundaries);

    printf("%i boundaries read\n",iBoundaries);
    if (iDebug == 1)
       printf("\nsorting boundary ids\n");

    iAlreadySorted = CreateIdIndex(iBoundaries,boundaries,&id1index);
    if (iAlreadySorted == 0)
    {
	   heapSort_id1(id1index,iBoundaries);

       printf("%i boundary ids sorted\n",iBoundaries);
    }
    else
        printf("boundaries already sorted\n");

    if (iDebug == 1)
       WriteId1(iBoundaries,id1index);

    free(boundaries);

    if (iDebug == 1)
       printf("\nreading river boundaries\n");

    ReadRiverBoundaryFile(sRiverBoundaryFileName,&iRiverBoundaries,&riverboundaries);

    if (iDebug == 1)
       WriteRiverBoundaryFile(iRiverBoundaries,riverboundaries);

    printf("%i river boundaries read\n",iRiverBoundaries);
    if (iDebug == 1)
       printf("\nreading weightings\n");

    ReadWeightingFile(sWeightingFileName,&iWeightings,&weightings);

    if (iDebug == 1)
       WriteWeightingFile(iWeightings,weightings);

    printf("%i weightings read\n",iWeightings);

    if (iDebug == 1)
       printf("\nsorting weightings\n");

    // check if weightings are already sorted before conducting sort
    iAlreadySorted = CreateSortedWeightings(iWeightings,weightings,&sortedweightings);
    if (iAlreadySorted == 0)
    {
       heapSort_weightings(sortedweightings,iWeightings);

       if (iDebug == 1)
          WriteSortedWeightings(iWeightings,sortedweightings);

       printf("%i weightings sorted\n",iWeightings);
    }
    else
        printf("weightings already sorted\n");

    free(weightings);

    if (iDebug == 1)
       printf("\ncomputing weighted boundaries\n");

    CreateComputeStuff(iRiverBoundaries,&computed,riverboundaries);
    ComputeWeightedBoundaries(iRiverBoundaries,iBoundaries,iWeightings,computed,
                              id1index,sortedweightings,sOutputFileName);

    if (iDebug == 1)
    {
       WriteComputeStuff(iRiverBoundaries,computed);
       WriteId1_end(iBoundaries,id1index);
    }

    printf("%i searches performed, %i weighted boundaries computed\n\nfinished\n",iSearchCount,iRiverBoundaries);

    free(riverboundaries);
    free(id1index);
    free(sortedweightings);
    free(computed);

    return 0;
}