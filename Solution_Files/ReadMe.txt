Download and install Oracle Virtual box from following link.

https://www.oracle.com/virtualization/solutions/try-oracle-vm-virtualbox/
 
Download HDP 2.5 Virtual Machinhe. This maching comes with pre-intalled Hadoop and Spark services.

https://www.cloudera.com/downloads/hortonworks-sandbox/hdp.html

Create Twitter developer account using following link and register the project.

https://developer.twitter.com/en/portal/dashboard

Start HDP machine on virtual box and open ssh shell at http://127.0.0.1:4200 or use ssh client like putty

Create flume.conf file(given with solution files)

switch to following path in ssh 
cd /usr/hdp/2.5.0.0-1245/flume/bin

Run command to extract tweets from twitter
flume-ng agent --conf /usr/hdp/current/flume-server/lib/flume-sources-1.0-SNAPSHOT.jar -f /root/flume.conf Dflume.root.logger=DEBUG,console -n tweetagent

## Sentiment analysis
// Once above steps are completed, run queries in Hive_Sentiment_Analysis.txt in a given sequence 

## Trend analysis
// Once above steps are completed, run queries in Trend_Analysis_Hive.txt and Trend_Analysis_pyspark.py in a given sequence 

## Move final data to google cloud
// perform steps given in Hadoop_Google_Cloud_Connection to set up connection and move data.

## Sentiment_analysis_result contains sample result data of sentiment analysis 