//
//  SiteListViewController.m
//
//  Created by guan mofeng on 12-1-12.
//  Copyright 2012 北京. All rights reserved.
//

#import "SiteListViewController.h"
#import "Dialog.h"
#include "GlobalData.h"


@implementation SiteListViewController
- (IBAction)btnOnClick:(id)sender {
	
    if([btnSegment selectedSegmentIndex] == 0) // Add
	{
		NSLog(@"Segment Add clicked: %d", btnSegment.selectedSegmentIndex);
		[self addSite];
		[tableSite reloadData];
		
		
	}
	else if([btnSegment selectedSegmentIndex] == 1) //Edit
	{
		NSLog(@"Segment Edit clicked: %d", btnSegment.selectedSegmentIndex);
		[self editSite];
		[tableSite reloadData];
		
	}else if([btnSegment selectedSegmentIndex] == 2) //delete
	{
		NSLog(@"Segment Delete clicked: %d", btnSegment.selectedSegmentIndex);
		[self deleteSite];
		[tableSite reloadData];
		
	}else if([btnSegment selectedSegmentIndex] == 3) //	connect
	{
		NSLog(@"Segment Connect clicked: %d", btnSegment.selectedSegmentIndex);
		[self connectSite];
		
	}else if([btnSegment selectedSegmentIndex] == 4) //	quit
	{
		NSLog(@"Segment Quit clicked: %d", btnSegment.selectedSegmentIndex);
		[self quitSite];
		
	}else {
		NSLog(@"Segment clicked: %d", btnSegment.selectedSegmentIndex);
	}
	
}

-(void) addSite {
	
	if(![self isValidator]){return;}
	SITEMAG site;
	LOCALREVBLK * plocal;
	int comp;
	
	
	sprintf(site.site,"%s",[txtName.text UTF8String]);
	
	sprintf(site.net_con.host,"%s",[txtAddress.text UTF8String]);
	sprintf(site.net_con.user,"%s",[txtUser.text UTF8String]);
	sprintf(site.net_con.passwd,"%s",[txtPassword.text UTF8String]);
	sscanf([txtPort.text UTF8String], "%d", &site.net_con.cmd_port);
	int chnum = 0;
	sscanf([txtChNum.text UTF8String], "%d", &chnum);
	
	comp=3;
	site.chn_sum = chnum ;
	 
	plocal=new LOCALREVBLK[site.chn_sum];
	int i = 0;
	for(int i=0;i<site.chn_sum;i++){
		plocal[i].chn_sum=comp;
		plocal[i].local_id=i;
		strcpy(plocal[i].obs_name,site.site);
	}
	for(i=0;i<site.chn_sum-1;i++)
		plocal[i].lpnext=&plocal[i+1];
	plocal[i].lpnext=NULL;
		
	siteDoc.m_sitelst.push_back((CSitelinkObj *)new CSitelinkObj(&site,plocal));
	//m_sitelst->AddTail((CSitelinkObj *)new CSitelinkObj(&site,plocal));
	
	//m_list_site.AddString(m_stn);
	[self saveSiteinfo];	
	
}


-(void) editSite {
	
	if(selectPos == -1)
		return;
	
	if(selectPos>=siteDoc.m_sitelst.size())
		return;
	
	if(![self isValidator]){return;}
	CSitelinkObj *pobj = siteDoc.m_sitelst.at(selectPos);
	delete pobj->m_plocal;
	
	/////////////

	sprintf(pobj->m_site.site,"%s",[txtName.text UTF8String]);
	
	sprintf(pobj->m_site.net_con.host,"%s",[txtAddress.text UTF8String]);
	sprintf(pobj->m_site.net_con.user,"%s",[txtUser.text UTF8String]);
	sprintf(pobj->m_site.net_con.passwd,"%s",[txtPassword.text UTF8String]);
	sscanf([txtPort.text UTF8String], "%d", &pobj->m_site.net_con.cmd_port);
	
	int chnum ;
	sscanf([txtChNum.text UTF8String], "%d", &chnum);
	 
	
	pobj->m_site.chn_sum = chnum ;
	int comp=3;
	
	LOCALREVBLK * plocal=new LOCALREVBLK[pobj->m_site.chn_sum];
	int i = 0;
	for(int i=0;i<pobj->m_site.chn_sum;i++){
		plocal[i].chn_sum=comp;
		plocal[i].local_id=i;
		strcpy(plocal[i].obs_name,pobj->m_site.site);
	}
	for(i=0;i<pobj->m_site.chn_sum-1;i++)
		plocal[i].lpnext=&plocal[i+1];
	plocal[i].lpnext=NULL;
	pobj->m_plocal=plocal;
	
	[self saveSiteinfo];
}
-(void) deleteSite {
	
	if(selectPos == -1)
		return;
	
	if(selectPos>=siteDoc.m_sitelst.size())
		return;
	
	CSitelinkObj *pobj=(CSitelinkObj *)siteDoc.m_sitelst.at(selectPos);
	if(pobj==NULL)
		return;
	delete pobj->m_plocal;
	delete pobj;
	
	//vector<CSitelinkObj *>::iterator iter = siteDoc.m_sitelst[selectPos];
	
	siteDoc.m_sitelst.erase(siteDoc.m_sitelst.begin() + selectPos);
	selectPos=-1;
	
	[self saveSiteinfo];
	
}



- (void) onConnectTimer : (NSTimer *)timer
{
	currConnetTime += 0.5f;
	if(!siteDoc.IsConnected())
	{
		if(currConnetTime >= 60) {
			[timer invalidate];
			currConnetTime = 0;
			[baseAlert dismissWithClickedButtonIndex:0 animated:NO];
			//[self.navigationController popViewControllerAnimated:YES];
			
			UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"连接超时，请检查连接配置" 
														 delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
			[av show];
			return;
			
		}else {
			return;
		}
	}
	[timer invalidate];
	currConnetTime = 0;
	[baseAlert dismissWithClickedButtonIndex:0 animated:NO];
	[self.navigationController popViewControllerAnimated:YES];
	
}


-(void) connectSite {
	
	if(selectPos == -1)
		return;
	
	if(selectPos>=siteDoc.m_sitelst.size())
		return;
	if(siteDoc.IsConnected())
	{ siteDoc.CloseCon(); return;}
	
	
	baseAlert = [[[UIAlertView alloc] initWithTitle:@"正在连接" message:nil 
							delegate:nil cancelButtonTitle:nil otherButtonTitles:nil] autorelease];
	[baseAlert show];
	
	UIActivityIndicatorView *aiv = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle: UIActivityIndicatorViewStyleWhiteLarge];
	aiv.center = CGPointMake(baseAlert.bounds.size.width /2.0f, baseAlert.bounds.size.height/2.0f);
	[aiv startAnimating];
	
	[baseAlert addSubview:aiv];
	[aiv release];
	
	
	m_benable_connect = 1;
	//[self.parentViewController dismissModalViewControllerAnimated:YES];
	//[[self parentViewController] dismissModalViewControllerAnimated:YES];
	//[self.navigationItem  ;
	//CSitelinkObj *pobj=(CSitelinkObj *)siteDoc.m_sitelst.at(selectPos);
	//siteDoc.OnRunSitemag(pobj);
	siteDoc.OnRunSitemag(selectPos);
	siteDoc.m_update_id=1;//set procsscon timer 
	
	
	m_timer = [NSTimer scheduledTimerWithTimeInterval:0.5f target:self selector:@selector(onConnectTimer:) userInfo:nil repeats:YES];
	currConnetTime = 0;
	
	
	
}
-(void) quitSite {
	[self.navigationController popViewControllerAnimated:YES];

}

-(BOOL) saveSiteinfo {
	
	FILE *f;
	
	int count,chn_sum;
	CSitelinkObj * pobj;
	LOCALREVBLK *pblk;
	string name(theApp.m_taskpath);
	name+="/cfg/sitelst.cfg";
	
	if((f = fopen(name.c_str(), "w")) == NULL)
	{return FALSE;}
	
	count=siteDoc.m_sitelst.size();
	char tmp[1500];
	for(int i=0;i<count;i++)
	{
		//pos=m_sitelst->FindIndex(i);
		pobj=(CSitelinkObj *)siteDoc.m_sitelst.at(i);
		
		chn_sum=0;
		pblk=pobj->m_plocal;
		while(pblk!=NULL){
			chn_sum+=pblk->chn_sum;
			pblk=pblk->lpnext;
		}
		/*格式：
		 名称	字符串	名称
		 通道数	整数	通道数中数值
		 网络地址	字符串	网络--地址
		 端口	整数	网络-端口
		 用户名	字符串	网络-用户名
		 密码	字符串	网络-密码
		 */
		sprintf(tmp, "%s %d %s %d %s %s\n",
 				pobj->m_site.site,chn_sum,pobj->m_site.net_con.host,pobj->m_site.net_con.cmd_port,pobj->m_site.net_con.user,pobj->m_site.net_con.passwd );
		//s.Format("%s %d %s %d %s %s\n",
 		//		pobj->m_site.site,chn_sum,pobj->m_site.net_con.host,pobj->m_site.net_con.cmd_port,pobj->m_site.net_con.user,pobj->m_site.net_con.passwd );
		//f.WriteString(s);
		//fwrite(buff);
		fprintf(f,"%s", tmp);
	}
	
	fclose(f);
	return TRUE;
	
}

-(BOOL) isValidator {
	if([txtName.text length] == 0){
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"Name is null" 
													delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return FALSE;
	}
	
	if([txtAddress.text length] == 0){
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"Address is null" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return FALSE;
	}
	
	if([txtUser.text length] == 0){
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"User is null" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return FALSE;
	}
	
	if([txtPassword.text length] == 0){
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"Password is null" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return FALSE;
	}
	
	if([txtPort.text length] == 0){
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"Port is null" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return FALSE;
	}
	
	
	if([txtChNum.text length] == 0){
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"Chnum is null" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return FALSE;
	}
		
	return TRUE;
}


- (void)viewDidLoad
{
	/*if(siteDoc.IsConnected())
	{
		siteDoc.CloseCon();
		[self.navigationController popViewControllerAnimated:YES];
	}*/
	btnSegment.momentary = YES;
	txtChNum.delegate = self;
	txtPort.delegate =self;
	txtUser.delegate=self;
	txtPassword.delegate=self;
	txtAddress.delegate=self;
	txtName.delegate=self;
	
	tableSite.dataSource = self;
	tableSite.delegate = self;
	selectPos = -1;
	 
    [tableSite reloadData];
	[btnSegment setEnabled:NO forSegmentAtIndex: 3];
	
	
}


#pragma mark -
#pragma mark UITableViewDelegate and UITableViewDatasource methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return siteDoc.m_sitelst.size();
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{        
	return 1;
}	

- (NSString *)tableView:(UITableView *)aTableView titleForHeaderInSection:(NSInteger)section 
{        
	return nil;
} 

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectMake(0.0f,0.0f, 120.0f,20.0f)]autorelease];  
	label.backgroundColor = [UIColor clearColor];
	label.text = @"台站列表";
	label.textColor = [UIColor grayColor];
	label.font =  [UIFont systemFontOfSize:10];
	label.textAlignment = UITextAlignmentCenter;  
	return label;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *kCellIdentifier = @"cellID";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kCellIdentifier];
	if (!cell)
	{
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:kCellIdentifier] autorelease];
        
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        
        cell.textLabel.backgroundColor = [UIColor clearColor];
		cell.textLabel.opaque = NO;
		cell.textLabel.textColor = [UIColor blackColor];
		cell.textLabel.highlightedTextColor = [UIColor whiteColor];
		cell.textLabel.font = [UIFont boldSystemFontOfSize:18];
		
		cell.detailTextLabel.backgroundColor = [UIColor clearColor];
		cell.detailTextLabel.opaque = NO;
		cell.detailTextLabel.textColor = [UIColor grayColor];
		cell.detailTextLabel.highlightedTextColor = [UIColor whiteColor];
		cell.detailTextLabel.font = [UIFont systemFontOfSize:14];
    }
    
	char * uu = siteDoc.m_sitelst.at(indexPath.row)->m_site.site;
	// get the view controller's info dictionary based on the indexPath's row
	//NSDictionary *dataDictionary = [menuList objectAtIndex:indexPath.row];
    //cell.textLabel.text = [dataDictionary valueForKey:kTitleKey];
	cell.textLabel.text = [[NSString alloc] initWithUTF8String:uu]; 
	//txtAddress.text = 
	
	return cell;
	
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {        
	return 40.0f;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {        
	
	[self OnSelchangeListSite:indexPath.row];
	
	//int i = indexPath.row;
	
	
	
	//selectRowData = [self.menuList objectAtIndex:indexPath.row];
	
}


- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

-(void) OnSelchangeListSite : (int) selIndex {
	
	//m_host=pobj->m_site.net_con.host;
	//m_port=pobj->m_site.net_con.cmd_port;
	//m_user=pobj->m_site.net_con.user;
	//m_passwd=pobj->m_site.net_con.passwd;
	selectPos = selIndex;
	CSitelinkObj *p = siteDoc.m_sitelst.at(selIndex);
	txtAddress.text = [[NSString alloc] initWithUTF8String:p->m_site.net_con.host]; 
	
	char temp[1000];
	sprintf(temp,"%d", p->m_site.net_con.cmd_port);
	txtName.text = [[NSString alloc] initWithUTF8String:p->m_site.site];
	txtPort.text = [[NSString alloc] initWithUTF8String:temp]; 
	
	sprintf(temp,"%d", p->m_site.chn_sum);
	txtChNum.text = [[NSString alloc] initWithUTF8String:temp]; 
	txtUser.text = [[NSString alloc] initWithUTF8String:p->m_site.net_con.user];
	txtPassword.text = [[NSString alloc] initWithUTF8String:p->m_site.net_con.passwd];
	[btnSegment setEnabled:YES forSegmentAtIndex: 3];
	
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	[textField resignFirstResponder];
	return YES;
}


@end
