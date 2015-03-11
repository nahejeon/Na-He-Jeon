class CommentsController < ApplicationController
	def new
	end
	def create
		if session[:current_user_id] == nil 
			redirect_to "/comments/new/" + params[:id].to_s
		else
			newcomment = Comment.new
			newcomment.photo_id = params[:id]
			newcomment.user_id = session[:current_user_id]
			newcomment.date_time = DateTime.now
			newcomment.comment = params[:comment]
			if newcomment.save()
				x = Photo.find_by_id(newcomment.photo_id)
				y = x.user_id
				redirect_to "/photos/index/" + y.to_s
			else 
				newcomment.destroy()
				flash[:danger] = "You cannot enter in an empty comment."
				redirect_to(:controller => "comments", :action => "new", :id => params[:id].to_s)
			end
		end
	end
end
